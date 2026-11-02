#include "logger.hpp"
#include "svg_writer.hpp"
#include "svg_node_writer.hpp"
#include "svg_common.hpp"
#include "component_type.hpp"
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

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
void write_svg_wire(FILE *f, const node &n1, const node &n2,
                    const std::vector<node> &nodes, int32_t idx1, int32_t idx2);

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
    int32_t x_border = 60;
    int32_t y_border = 60;


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
            "        .wire { fill: none; stroke: #000000; stroke-width: 2; stroke-linejoin: round }\n"
            "        .pin-wire { fill: none; stroke-width: 2; stroke-linejoin: round }\n"
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

// --- Bounding box and collision helpers ---

struct BBox
{
    int32_t x_min, y_min, x_max, y_max;
};

// Get the visual bounding box of a component node (with margin)
BBox get_node_bbox(const node &n)
{
    constexpr int32_t M = 5; // margin around components
    switch(n.comp_type)
    {
        case component_type::R:
            if (n.rotation == 0 || n.rotation == 180)
                return {n.x - 10 - M, n.y - 20 - M, n.x + 10 + M, n.y + 20 + M};
            else
                return {n.x - 20 - M, n.y - 10 - M, n.x + 20 + M, n.y + 10 + M};
        case component_type::C:
            if (n.rotation == 0 || n.rotation == 180)
                return {n.x - 10 - M, n.y - 5 - M, n.x + 10 + M, n.y + 5 + M};
            else
                return {n.x - 5 - M, n.y - 10 - M, n.x + 5 + M, n.y + 10 + M};
        case component_type::L:
            if (n.rotation == 0 || n.rotation == 180)
                return {n.x - 8 - M, n.y - 18 - M, n.x + 8 + M, n.y + 18 + M};
            else
                return {n.x - 18 - M, n.y - 8 - M, n.x + 18 + M, n.y + 8 + M};
        case component_type::V:
        case component_type::I:
            return {n.x - 18 - M, n.y - 18 - M, n.x + 18 + M, n.y + 18 + M};
        case component_type::D:
            return {n.x - 15 - M, n.y - 15 - M, n.x + 15 + M, n.y + 15 + M};
        case component_type::M:
            // Conservative square bbox that works for any rotation
            return {n.x - 30 - M, n.y - 30 - M, n.x + 30 + M, n.y + 30 + M};
        case component_type::Q:
            return {n.x - 25 - M, n.y - 25 - M, n.x + 25 + M, n.y + 25 + M};
        default:
            return {n.x, n.y, n.x, n.y};
    }
}

// Check if horizontal segment (xa,y)->(xb,y) intersects bounding box
bool h_seg_hits_bbox(int32_t xa, int32_t xb, int32_t y, const BBox &bb)
{
    if (y < bb.y_min || y > bb.y_max) return false;
    int32_t x_lo = std::min(xa, xb);
    int32_t x_hi = std::max(xa, xb);
    return x_hi >= bb.x_min && x_lo <= bb.x_max;
}

// Check if vertical segment (x,ya)->(x,yb) intersects bounding box
bool v_seg_hits_bbox(int32_t x, int32_t ya, int32_t yb, const BBox &bb)
{
    if (x < bb.x_min || x > bb.x_max) return false;
    int32_t y_lo = std::min(ya, yb);
    int32_t y_hi = std::max(ya, yb);
    return y_hi >= bb.y_min && y_lo <= bb.y_max;
}

// Count how many component bboxes an L-shaped wire path hits
// h_first=true: horizontal then vertical; false: vertical then horizontal
int count_l_wire_hits(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                       bool h_first, const std::vector<node> &nodes,
                       int32_t skip_i, int32_t skip_j)
{
    int hits = 0;
    for (size_t k = 0; k < nodes.size(); ++k)
    {
        if (static_cast<int32_t>(k) == skip_i || static_cast<int32_t>(k) == skip_j)
            continue;
        const auto &nd = nodes[k];
        if (nd.comp_type == component_type::DotPoint || nd.comp_type == component_type::Ground)
            continue;
        
        BBox bb = get_node_bbox(nd);
        if (h_first)
        {
            // H segment: (x1,y1)→(x2,y1), then V segment: (x2,y1)→(x2,y2)
            if (h_seg_hits_bbox(x1, x2, y1, bb) || v_seg_hits_bbox(x2, y1, y2, bb))
                ++hits;
        }
        else
        {
            // V segment: (x1,y1)→(x1,y2), then H segment: (x1,y2)→(x2,y2)
            if (v_seg_hits_bbox(x1, y1, y2, bb) || h_seg_hits_bbox(x1, x2, y2, bb))
                ++hits;
        }
    }
    return hits;
}

// --- Wire drawing helpers ---

// Draw an orthogonal wire between two points, choosing L-direction to avoid collisions
void write_svg_orthogonal_wire(FILE *f, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                               const std::vector<node> &nodes, int32_t skip_i, int32_t skip_j,
                               const char *color = "#000000")
{
    if (x1 == x2 || y1 == y2)
    {
        // Straight line
        fprintf(f, "        <path class=\"pin-wire\" d=\"M %d %d L %d %d\" stroke=\"%s\" />\n",
                   x1, y1, x2, y2, color);
    }
    else
    {
        // Try both L-directions, pick the one with fewer collisions
        int h_hits = count_l_wire_hits(x1, y1, x2, y2, true, nodes, skip_i, skip_j);
        int v_hits = count_l_wire_hits(x1, y1, x2, y2, false, nodes, skip_i, skip_j);
        
        if (v_hits < h_hits)
        {
            // Vertical first, then horizontal
            fprintf(f, "        <path class=\"pin-wire\" d=\"M %d %d L %d %d L %d %d\" stroke=\"%s\" />\n",
                       x1, y1, x1, y2, x2, y2, color);
        }
        else
        {
            // Horizontal first, then vertical (default)
            fprintf(f, "        <path class=\"pin-wire\" d=\"M %d %d L %d %d L %d %d\" stroke=\"%s\" />\n",
                       x1, y1, x2, y1, x2, y2, color);
        }
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
                
                write_svg_wire(f, n1, n2, nodes, i, j);
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
        
        // Collect all connected nodes from graph edges
        std::vector<int32_t> connected_nodes;
        for (int32_t j = 0; j < g.n_node; ++j)
        {
            if (g.edge_of(static_cast<int32_t>(t_idx), j))
            {
                connected_nodes.push_back(j);
            }
        }
        
        // Track which connected nodes have been assigned to a pin
        std::vector<bool> assigned(connected_nodes.size(), false);
        
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
            if (net_idx < 0)
            {
                for (size_t ci = 0; ci < connected_nodes.size(); ++ci)
                {
                    if (assigned[ci]) continue;
                    int32_t conn_idx = connected_nodes[ci];
                    const node &conn_node = nodes[conn_idx];
                    if (conn_node.comp_type == component_type::DotPoint ||
                        conn_node.comp_type == component_type::Ground)
                    {
                        if (strncmp(conn_node.name, transistor.pin_nets[pin], 7) == 0)
                        {
                            net_idx = conn_idx;
                            assigned[ci] = true;
                            break;
                        }
                    }
                }
            }
            
            // Fallback: use positional heuristics based on pin direction (rotation-aware)
            if (net_idx < 0 && !connected_nodes.empty())
            {
                // Compute which direction this pin extends (in rotated space)
                int32_t pin_dir_x = 0, pin_dir_y = 0;
                if (transistor.comp_type == component_type::M)
                {
                    if (pin == 0) { pin_dir_y = -1; }       // Drain: up
                    else if (pin == 1) { pin_dir_x = -1; }  // Gate: left
                    else { pin_dir_y = 1; }                  // Source: down
                }
                else // BJT
                {
                    if (pin == 0) { pin_dir_x = 1; pin_dir_y = -1; }  // Collector: top-right
                    else if (pin == 1) { pin_dir_x = -1; }             // Base: left
                    else { pin_dir_x = 1; pin_dir_y = 1; }             // Emitter: bottom-right
                }
                svg::rotate_offset(pin_dir_x, pin_dir_y, transistor.rotation);
                
                for (size_t ci = 0; ci < connected_nodes.size(); ++ci)
                {
                    if (assigned[ci]) continue;
                    int32_t conn_idx = connected_nodes[ci];
                    const node &conn_node = nodes[conn_idx];
                    if (conn_node.comp_type != component_type::DotPoint &&
                        conn_node.comp_type != component_type::Ground)
                        continue;
                    
                    int32_t rel_x = conn_node.x - transistor.x;
                    int32_t rel_y = conn_node.y - transistor.y;
                    
                    bool matches = true;
                    if (pin_dir_x > 0) matches = matches && (rel_x > 0);
                    if (pin_dir_x < 0) matches = matches && (rel_x < 0);
                    if (pin_dir_y > 0) matches = matches && (rel_y > 0);
                    if (pin_dir_y < 0) matches = matches && (rel_y < 0);
                    
                    if (matches)
                    {
                        net_idx = conn_idx;
                        assigned[ci] = true;
                        break;
                    }
                }
            }
            
            if (net_idx < 0) continue;
            
            const node &net_node = nodes[net_idx];
            
            // Calculate pin position with rotation support
            int32_t pin_x, pin_y;
            int32_t dx = 0, dy = 0;
            if (transistor.comp_type == component_type::M)
            {
                constexpr int32_t BODY_W = 24;
                constexpr int32_t BODY_H = 30;
                if (pin == 0) { dx = 0; dy = -(BODY_H/2 + 10); }       // Drain
                else if (pin == 1) { dx = -(BODY_W/2 + 15); dy = 0; }  // Gate
                else { dx = 0; dy = BODY_H/2 + 10; }                    // Source
            }
            else  // BJT
            {
                constexpr int32_t SIZE = 20;
                if (pin == 0) { dx = SIZE; dy = -SIZE; }       // Collector
                else if (pin == 1) { dx = -SIZE; dy = 0; }     // Base
                else { dx = SIZE; dy = SIZE; }                  // Emitter
            }
            svg::rotate_offset(dx, dy, transistor.rotation);
            pin_x = transistor.x + dx;
            pin_y = transistor.y + dy;
            
            // Draw wire from pin to net node with pin-specific color
            const char *wire_color = get_pin_color(pin);
            write_svg_orthogonal_wire(f, pin_x, pin_y, net_node.x, net_node.y,
                                       nodes, static_cast<int32_t>(t_idx), net_idx, wire_color);
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

void write_svg_wire(FILE *f, const node &n1, const node &n2,
                    const std::vector<node> &nodes, int32_t idx1, int32_t idx2)
{
    // If already aligned horizontally or vertically, draw single line
    if (n1.x == n2.x || n1.y == n2.y)
    {
        fprintf(f, "        <path class=\"wire\" d=\"M %d %d L %d %d\" />\n",
                   n1.x, n1.y, n2.x, n2.y);
    }
    else
    {
        // Try both L-directions, pick the one with fewer component collisions
        int h_hits = count_l_wire_hits(n1.x, n1.y, n2.x, n2.y, true, nodes, idx1, idx2);
        int v_hits = count_l_wire_hits(n1.x, n1.y, n2.x, n2.y, false, nodes, idx1, idx2);
        
        if (v_hits < h_hits)
        {
            // Vertical first, then horizontal
            fprintf(f, "        <path class=\"wire\" d=\"M %d %d L %d %d L %d %d\" />\n",
                       n1.x, n1.y, n1.x, n2.y, n2.x, n2.y);
        }
        else
        {
            // Horizontal first, then vertical (default)
            fprintf(f, "        <path class=\"wire\" d=\"M %d %d L %d %d L %d %d\" />\n",
                       n1.x, n1.y, n2.x, n1.y, n2.x, n2.y);
        }
    }
}
}

}

