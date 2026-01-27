#include "logger.hpp"
#include "svg_writer.hpp"
#include "svg_node_writer.hpp"
#include "svg_common.hpp"
#include "component_type.hpp"
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>

namespace svg
{

namespace
{
void begin_svg(FILE *f, const std::vector<node> &nodes);
void end_svg(FILE *f);
void write_svg_wires(FILE *f, const graph &g, const std::vector<node> &nodes);
void write_svg_transistors(FILE *f, const graph &g, const std::vector<node> &nodes);
void write_svg_other_nodes(FILE *f, const graph &g, const std::vector<node> &nodes);
}

void write_layout_svg(const graph &g, const std::vector<node> &nodes, const char *filename)
{
    std::string s(filename);
    s.append(".svg");
    FILE *f = fopen(s.c_str(), "w");
    if (f == nullptr)
    {
        LOG_ERROR("Cannot open file %s", filename);
        return;
    }

    begin_svg(f, nodes);
    write_svg_transistors(f, g, nodes);  // 1. Draw transistors first (background)
    write_svg_wires(f, g, nodes);         // 2. Draw wires on top of transistors
    write_svg_other_nodes(f, g, nodes);   // 3. Draw other components on top of wires
    end_svg(f);

    fclose(f);
}

namespace
{
void write_svg_wire(FILE *f, const node &n1, const node &n2);

void begin_svg(FILE *f, const std::vector<node> &nodes)
{
    int32_t min_x = nodes[0].x;
    int32_t max_x = nodes[0].x;
    int32_t min_y = nodes[0].y;
    int32_t max_y = nodes[0].y;

    for (const auto &n : nodes)
    {
        min_x = (n.x < min_x) ? n.x : min_x;
        max_x = (n.x > max_x) ? n.x : max_x;
        min_y = (n.y < min_y) ? n.y : min_y;
        max_y = (n.y > max_y) ? n.y : max_y;
    }

    int32_t dx = max_x - min_x;
    int32_t dy = max_y - min_y;
    int32_t x_border = 40;
    int32_t y_border = 40;


    int32_t width = dx + 2 * x_border;
    int32_t height = dy + 2 * y_border;
    int32_t x0 = min_x - x_border;
    int32_t y0 = min_y - y_border;

    fprintf(f, "<?xml version=\"1.0\"?>\n"
            "<svg\n"
            "    xmlns=\"http://www.w3.org/2000/svg\"\n"
            "    xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
            "    xmlns:ev=\"http://www.w3.org/2001/xml-events\"\n"
            "    version=\"1.1\"\n"
            "    baseProfile=\"full\"\n"
            "    viewBox=\"%d %d %d %d\">\n"
            "    <style>\n"
            "        .vert { font-size: %d; fill: #000000; dominant-baseline: middle }\n"
            "        .hori { font-size: %d; fill: #000000; text-anchor: middle }\n"
            "    </style>\n",
            x0, y0, width, height,
            svg::font_size, svg::font_size
           );

    // for drawing arrows in voltage sources
    fprintf(f, "    <defs>\n"
               "        <marker id=\"arrowhead\" markerWidth=\"8\" markerHeight=\"8\" refX=\"0\" refY=\"4\" orient=\"auto\">\n"
               "        <polygon points=\"0 0, 4 4, 0 8\" />\n"
               "        </marker>\n"
               "    </defs>\n");

    // force white background
    fprintf(f, "    <g>\n"
               "        <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:#ffffff\" />\n"
               "    </g>\n",
               x0, y0, width, height);
}

void end_svg(FILE *f)
{
    fprintf(f, "</svg>");
}

// Helper to draw orthogonal SVG wire between two points
void write_svg_orthogonal_wire(FILE *f, int32_t x1, int32_t y1, int32_t x2, int32_t y2, const char *color = "#000000")
{
    if (x1 == x2 || y1 == y2)
    {
        fprintf(f, "        <g>\n"
                   "            <path fill=\"none\" d=\"M %d %d L %d %d\" stroke=\"%s\" stroke-width=\"2\" />\n"
                   "        </g>\n",
                   x1, y1, x2, y2, color);
    }
    else
    {
        // L-shaped: horizontal first, then vertical
        fprintf(f, "        <g>\n"
                   "            <path fill=\"none\" d=\"M %d %d L %d %d L %d %d\" stroke=\"%s\" stroke-width=\"2\" />\n"
                   "        </g>\n",
                   x1, y1, x2, y1, x2, y2, color);
    }
}

// Pin colors: Drain/Collector=red, Gate/Base=blue, Source/Emitter=green
const char* get_pin_color(int pin)
{
    switch(pin)
    {
        case 0: return "#CC0000";  // Drain/Collector - Red
        case 1: return "#0000CC";  // Gate/Base - Blue
        case 2: return "#00AA00";  // Source/Emitter - Green
        default: return "#000000"; // Black
    }
}

void write_svg_wires(FILE *f, const graph &g, const std::vector<node> &nodes)
{
    fprintf(f, "    <g>\n");
    
    // Draw regular wires (skip transistor edges)
    for (int32_t i = 0; i < g.n_node; ++i)
    {
        for (int32_t j = 0; j < i; ++j)
        {
            if (g.edge_of(i, j))
            {
                const node &n1 = nodes[i];
                const node &n2 = nodes[j];
                
                // Skip if either node is a transistor
                if (n1.comp_type == component_type::M || n1.comp_type == component_type::Q ||
                    n2.comp_type == component_type::M || n2.comp_type == component_type::Q)
                {
                    continue;
                }
                
                write_svg_wire(f, n1, n2);
            }
        }
    }
    
    // Draw transistor pin wires separately
    for (size_t t_idx = 0; t_idx < nodes.size(); ++t_idx)
    {
        const auto &transistor = nodes[t_idx];
        if (transistor.comp_type != component_type::M && transistor.comp_type != component_type::Q)
        {
            continue;
        }
        
        // SVG transistor symbol dimensions (must match svg_node_writer.cpp)
        // MOSFET: BODY_W=24, BODY_H=30, gate at x-BODY_W/2-15, drain at y-BODY_H/2-10, source at y+BODY_H/2+10
        // BJT: SIZE=20, collector at (x+SIZE, y-SIZE), base at (x-SIZE, y), emitter at (x+SIZE, y+SIZE)
        
        // Collect all connected nodes from graph edges
        std::vector<int32_t> connected_nodes;
        for (int32_t j = 0; j < g.n_node; ++j)
        {
            if (g.edge_of(static_cast<int32_t>(t_idx), j))
            {
                connected_nodes.push_back(j);
            }
        }
        
        for (int pin = 0; pin < transistor.n_pins && pin < 4; ++pin)
        {
            // Skip bulk pin if same as source (MOSFET)
            if (transistor.comp_type == component_type::M && pin == 3 &&
                strncmp(transistor.pin_nets[3], transistor.pin_nets[2], 7) == 0)
            {
                continue;
            }
            
            // Find the net node for this pin - first try by name
            int32_t net_idx = node_find_idx_by_name(nodes, transistor.pin_nets[pin]);
            
            // If not found by name, search through connected graph edges
            // The layoutizer may have created intermediate nodes with different names
            if (net_idx < 0)
            {
                // Look through all nodes connected via graph edges
                for (int32_t conn_idx : connected_nodes)
                {
                    const node &conn_node = nodes[conn_idx];
                    // Check if this connected node's name matches or if it's a DotPoint
                    // that might be an intermediate node for this pin's net
                    if (conn_node.comp_type == component_type::DotPoint ||
                        conn_node.comp_type == component_type::Ground)
                    {
                        // Check if this node was created from this pin's net
                        // by checking if the original net name is a substring or matches
                        if (strncmp(conn_node.name, transistor.pin_nets[pin], 7) == 0)
                        {
                            net_idx = conn_idx;
                            break;
                        }
                    }
                }
            }
            
            // If still not found, use the first available connected DotPoint for this pin
            // based on pin ordering heuristics
            if (net_idx < 0 && !connected_nodes.empty())
            {
                // Sort connected nodes by position to match pins
                // Drain/Collector = top (min y), Source/Emitter = bottom (max y), Gate/Base = left (min x)
                for (int32_t conn_idx : connected_nodes)
                {
                    const node &conn_node = nodes[conn_idx];
                    if (conn_node.comp_type != component_type::DotPoint &&
                        conn_node.comp_type != component_type::Ground)
                        continue;
                    
                    if (transistor.comp_type == component_type::M)
                    {
                        // MOSFET: pin 0=Drain(top), pin 1=Gate(left), pin 2=Source(bottom)
                        if (pin == 0 && conn_node.y < transistor.y) { net_idx = conn_idx; break; }
                        if (pin == 1 && conn_node.x < transistor.x) { net_idx = conn_idx; break; }
                        if (pin == 2 && conn_node.y > transistor.y) { net_idx = conn_idx; break; }
                    }
                    else
                    {
                        // BJT: pin 0=Collector(top), pin 1=Base(left), pin 2=Emitter(bottom)
                        if (pin == 0 && conn_node.y < transistor.y) { net_idx = conn_idx; break; }
                        if (pin == 1 && conn_node.x < transistor.x) { net_idx = conn_idx; break; }
                        if (pin == 2 && conn_node.y > transistor.y) { net_idx = conn_idx; break; }
                    }
                }
            }
            
            if (net_idx < 0) continue;
            
            const node &net_node = nodes[net_idx];
            
            // Calculate pin position based on transistor type
            int32_t pin_x, pin_y;
            if (transistor.comp_type == component_type::M)
            {
                // MOSFET pins - must match svg_node_writer.cpp exactly
                // BODY_W=24, BODY_H=30
                // Gate lead ends at: (x - BODY_W/2 - 15, y) = (x - 27, y)
                // Drain lead ends at: (x, y - BODY_H/2 - 10) = (x, y - 25)
                // Source lead ends at: (x, y + BODY_H/2 + 10) = (x, y + 25)
                constexpr int32_t BODY_W = 24;
                constexpr int32_t BODY_H = 30;
                if (pin == 0) {  // Drain (top)
                    pin_x = transistor.x;
                    pin_y = transistor.y - BODY_H/2 - 10;  // y - 25
                }
                else if (pin == 1) {  // Gate (left)
                    pin_x = transistor.x - BODY_W/2 - 15;  // x - 27
                    pin_y = transistor.y;
                }
                else {  // Source (bottom)
                    pin_x = transistor.x;
                    pin_y = transistor.y + BODY_H/2 + 10;  // y + 25
                }
            }
            else  // BJT
            {
                // BJT pins - must match svg_node_writer.cpp exactly
                // SIZE=20
                // Collector lead ends at: (x + SIZE, y - SIZE) = (x + 20, y - 20)
                // Base lead ends at: (x - SIZE, y) = (x - 20, y)
                // Emitter lead ends at: (x + SIZE, y + SIZE) = (x + 20, y + 20)
                constexpr int32_t SIZE = 20;
                if (pin == 0) {  // Collector (top-right)
                    pin_x = transistor.x + SIZE;
                    pin_y = transistor.y - SIZE;
                }
                else if (pin == 1) {  // Base (left)
                    pin_x = transistor.x - SIZE;
                    pin_y = transistor.y;
                }
                else {  // Emitter (bottom-right)
                    pin_x = transistor.x + SIZE;
                    pin_y = transistor.y + SIZE;
                }
            }
            
            // Draw wire from pin to net node with pin-specific color
            const char *wire_color = get_pin_color(pin);
            write_svg_orthogonal_wire(f, pin_x, pin_y, net_node.x, net_node.y, wire_color);
        }
    }
    
    fprintf(f, "    </g>\n");
}

void write_svg_component_node(FILE *f, const node &n)
{
    switch(n.comp_type)
    {
        case component_type::R:
        {
            write_svg_resistor(f, n);
            break;
        }
        case component_type::L:
        {
            write_svg_inductor(f, n);
            break;
        }
        case component_type::C:
        {
            write_svg_capacitor(f, n);
            break;
        }
        case component_type::V:
        {
            write_svg_voltage(f, n);
            break;
        }
        case component_type::I:
        {
            write_svg_current(f, n);
            break;
        }
        default:
            LOG_WARN("Type of node %s not yet implemented", n.name);
    }
}

void write_svg_transistors(FILE *f, const graph &g, const std::vector<node> &nodes)
{
    for (int32_t i = 0; i < g.n_node; ++i)
    {
        const auto &n = nodes[i];
        if (n.comp_type == component_type::Q)
        {
            write_svg_transistor(f, n);
        }
        else if (n.comp_type == component_type::M)
        {
            write_svg_mosfet(f, n);
        }
    }
}

void write_svg_other_nodes(FILE *f, const graph &g, const std::vector<node> &nodes)
{
    for (int32_t i = 0; i < g.n_node; ++i)
    {
        const auto &n = nodes[i];
        switch(n.comp_type)
        {
            case component_type::R:
                write_svg_resistor(f, n);
                break;
            case component_type::L:
                write_svg_inductor(f, n);
                break;
            case component_type::C:
                write_svg_capacitor(f, n);
                break;
            case component_type::V:
                write_svg_voltage(f, n);
                break;
            case component_type::I:
                write_svg_current(f, n);
                break;
            case component_type::D:
                write_svg_diode(f, n);
                break;
            case component_type::Ground:
                write_svg_ground(f, n);
                break;
            case component_type::DotPoint:
                if (g.count_neighbours(i) > 2)
                {
                    write_svg_dot(f, n);
                }
                break;
            case component_type::Q:
            case component_type::M:
                // Already drawn in write_svg_transistors
                break;
            default:
                LOG_WARN("Type of node %s not yet implemented", n.name);
        }
    }
}

void write_svg_wire(FILE *f, const node &n1, const node &n2)
{
    // If already aligned horizontally or vertically, draw single line
    if (n1.x == n2.x || n1.y == n2.y)
    {
        fprintf(f, "        <g>\n"
                   "            <path fill=\"none\" d=\"M %d %d L %d %d\" stroke=\"#000000\" />\n"
                   "        </g>\n",
                   n1.x, n1.y, n2.x, n2.y);
    }
    else
    {
        // Draw L-shaped orthogonal wire (horizontal first, then vertical)
        fprintf(f, "        <g>\n"
                   "            <path fill=\"none\" d=\"M %d %d L %d %d L %d %d\" stroke=\"#000000\" />\n"
                   "        </g>\n",
                   n1.x, n1.y, n2.x, n1.y, n2.x, n2.y);
    }
}
}

}

