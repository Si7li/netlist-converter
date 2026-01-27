#include "asc_writer.hpp"
#include "component_type.hpp"
#include "logger.hpp"
#include <string.h>

namespace
{
constexpr int32_t scale = 3;
const char* const symbol_map[n_component_type] = 
{
    "voltage",
    "res",
    "cap",
    "ind",
    "current",
    "npn",        // BJT transistor
    "nmos",       // MOSFET (will handle nmos/pmos dynamically)
    "diode"       // Diode
};
}

void write_header(FILE *f)
{
    fprintf(f, "Version 4\n"
               "SHEET 1 680 480\n");
}

void write_node_symbol(FILE *f, const node &n)
{
    int32_t x = scale * n.x;
    int32_t y = scale * n.y;

    // For MOSFETs: position symbol so Gate pin (at offset 0,80) aligns with node coordinate
    // Symbol position = node position - gate offset
    if (n.comp_type == component_type::M)
    {
        // Gate is at (symbol_x + 0, symbol_y + 80)
        // We want gate at (x, y), so symbol at (x - 0, y - 80) = (x, y - 80)
        y = y - 80;
    }
    // For BJTs: position symbol so Base pin (at offset 0,48) aligns with node coordinate
    else if (n.comp_type == component_type::Q)
    {
        // Base is at (symbol_x + 0, symbol_y + 48)
        // We want base at (x, y), so symbol at (x - 0, y - 48) = (x, y - 48)
        y = y - 48;
    }
    else if (!rotation_matters(n.comp_type))
    {
        x = n.rotation == 0 ? x - 16 : x;
        y = n.rotation == 90 ? y - 16 : y;
    }
    
    // For MOSFETs, determine if it's NMOS or PMOS based on model name
    const char *symbol = symbol_map[static_cast<int32_t>(n.comp_type)];
    if (n.comp_type == component_type::M)
    {
        // Check if model name contains "PMOS" or starts with 'P'
        if (strstr(n.val, "PMOS") || strstr(n.val, "pmos") || 
            strstr(n.val, "Pmos") || n.val[0] == 'P' || n.val[0] == 'p')
        {
            symbol = "pmos";
        }
        else
        {
            symbol = "nmos";
        }
    }
    // For BJTs, determine if it's NPN or PNP based on model name
    else if (n.comp_type == component_type::Q)
    {
        if (strstr(n.val, "PNP") || strstr(n.val, "pnp") || 
            strstr(n.val, "Pnp") || n.val[0] == 'P' || n.val[0] == 'p')
        {
            symbol = "pnp";
        }
        else
        {
            symbol = "npn";
        }
    }
    
    fprintf(f, "SYMBOL %s %d %d R%d\n"
               "SYMATTR InstName %s\n"
               "SYMATTR Value %s\n",
               symbol, x, y, n.rotation,
               n.name, n.val);
}

void write_gnd_node(FILE *f, const node &n)
{
    fprintf(f, "FLAG %d %d 0\n", scale * n.x, scale * n.y);
}

void write_wire(FILE *f, const node &n1, const node &n2, const std::vector<node> &nodes)
{
    int32_t offset_x[2] {}, offset_y[2] {};
    int32_t mul[2] {};
    
    int32_t x1 = scale * n1.x;
    int32_t y1 = scale * n1.y;
    int32_t x2 = scale * n2.x;
    int32_t y2 = scale * n2.y;

    // Check if n1 is a transistor - if so, find which pin n2 connects to
    if (n1.comp_type == component_type::M || n1.comp_type == component_type::Q)
    {
        // Transistor symbol position (adjusted for gate/base alignment)
        int32_t sym_x = x1;
        int32_t sym_y = (n1.comp_type == component_type::M) ? (y1 - 80) : (y1 - 48);
        
        for (int pin = 0; pin < n1.n_pins && pin < 4; ++pin)
        {
            if (strncmp(n2.name, n1.pin_nets[pin], sizeof(n2.name)) == 0)
            {
                if (n1.comp_type == component_type::M)
                {
                    // MOSFET pins: D(48,0), G(0,80), S(48,96)
                    if (pin == 0) { x1 = sym_x + 48; y1 = sym_y + 0; }       // Drain
                    else if (pin == 1) { x1 = sym_x + 0; y1 = sym_y + 80; }  // Gate
                    else { x1 = sym_x + 48; y1 = sym_y + 96; }               // Source/Bulk
                }
                else
                {
                    // BJT pins: C(64,0), B(0,48), E(64,96)
                    if (pin == 0) { x1 = sym_x + 64; y1 = sym_y + 0; }       // Collector
                    else if (pin == 1) { x1 = sym_x + 0; y1 = sym_y + 48; }  // Base
                    else { x1 = sym_x + 64; y1 = sym_y + 96; }               // Emitter
                }
                break;
            }
        }
    }
    else if (static_cast<int32_t>(n1.comp_type) < n_component_type)
    {
        mul[0] = 1;
    }

    // Check if n2 is a transistor - if so, find which pin n1 connects to
    if (n2.comp_type == component_type::M || n2.comp_type == component_type::Q)
    {
        // Transistor symbol position (adjusted for gate/base alignment)
        int32_t sym_x = scale * n2.x;
        int32_t sym_y = (n2.comp_type == component_type::M) ? (scale * n2.y - 80) : (scale * n2.y - 48);
        
        for (int pin = 0; pin < n2.n_pins && pin < 4; ++pin)
        {
            if (strncmp(n1.name, n2.pin_nets[pin], sizeof(n1.name)) == 0)
            {
                if (n2.comp_type == component_type::M)
                {
                    // MOSFET pins: D(48,0), G(0,80), S(48,96)
                    if (pin == 0) { x2 = sym_x + 48; y2 = sym_y + 0; }       // Drain
                    else if (pin == 1) { x2 = sym_x + 0; y2 = sym_y + 80; }  // Gate
                    else { x2 = sym_x + 48; y2 = sym_y + 96; }               // Source/Bulk
                }
                else
                {
                    // BJT pins: C(64,0), B(0,48), E(64,96)
                    if (pin == 0) { x2 = sym_x + 64; y2 = sym_y + 0; }       // Collector
                    else if (pin == 1) { x2 = sym_x + 0; y2 = sym_y + 48; }  // Base
                    else { x2 = sym_x + 64; y2 = sym_y + 96; }               // Emitter
                }
                break;
            }
        }
    }
    else if (static_cast<int32_t>(n2.comp_type) < n_component_type)
    {
        mul[1] = 1;
    }

    // Apply offsets for standard 2-pin components
    if (n1.x == n2.x)
    {
        if (n1.y > n2.y)
        {
            offset_y[0] = 16;
        }
        else
        {
            offset_y[1] = 16;
        }
    }
    else
    {
        if (n1.x < n2.x)
        {
            offset_x[0] = -16;
        }
        else
        {
            offset_x[1] = -16;
        }
    }
    offset_x[0] *= mul[0];
    offset_x[1] *= mul[1];
    offset_y[0] *= mul[0];
    offset_y[1] *= mul[1];
    
    fprintf(f, "WIRE %d %d %d %d\n", x1 + offset_x[0], y1 + offset_y[0],
                                     x2 + offset_x[1], y2 + offset_y[1]);
}

// Helper: Draw orthogonal (L-shaped) wire between two points
// Uses horizontal-first or vertical-first routing based on cleaner layout
void write_orthogonal_wire(FILE *f, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    // If already aligned, draw single wire
    if (x1 == x2 || y1 == y2)
    {
        fprintf(f, "WIRE %d %d %d %d\n", x1, y1, x2, y2);
        return;
    }
    
    // Draw L-shaped wire: horizontal first, then vertical
    // This creates cleaner layouts with wires running along edges
    fprintf(f, "WIRE %d %d %d %d\n", x1, y1, x2, y1);  // Horizontal segment
    fprintf(f, "WIRE %d %d %d %d\n", x2, y1, x2, y2);  // Vertical segment
}

void write_layout(FILE *f, const graph &g, const std::vector<node> &nodes)
{
    write_header(f);
    
    // Write all symbols
    for (const auto &n : nodes)
    {
        if (static_cast<int32_t>(n.comp_type) < n_component_type)
        {
            write_node_symbol(f, n);
        }
        else if (n.comp_type == component_type::Ground)
        {
            write_gnd_node(f, n);
        }
    }

    // Write wires - skip edges directly involving transistors (we'll handle them separately)
    for (int32_t i = 0; i < g.n_node; ++i)
    {
        for (int32_t j = 0; j < i; ++j)
        {
            if (g.edge_of(i, j))
            {
                const node &n1 = nodes[i];
                const node &n2 = nodes[j];
                
                // Skip if either node is a transistor (we handle transistor wires separately)
                if (n1.comp_type == component_type::M || n1.comp_type == component_type::Q ||
                    n2.comp_type == component_type::M || n2.comp_type == component_type::Q)
                {
                    continue;
                }
                
                write_wire(f, n1, n2, nodes);
            }
        }
    }
    
    // For each transistor, draw wires from each pin to its corresponding net node
    for (const auto &transistor : nodes)
    {
        if (transistor.comp_type != component_type::M && transistor.comp_type != component_type::Q)
        {
            continue;
        }
        
        // Calculate transistor symbol position (as written in write_node_symbol)
        int32_t sym_x = scale * transistor.x;
        int32_t sym_y = (transistor.comp_type == component_type::M) ? 
                        (scale * transistor.y - 80) : (scale * transistor.y - 48);
        
        // Draw wire for each pin
        for (int pin = 0; pin < transistor.n_pins && pin < 4; ++pin)
        {
            // Skip bulk pin if same as source
            if (transistor.comp_type == component_type::M && pin == 3 &&
                strncmp(transistor.pin_nets[3], transistor.pin_nets[2], 7) == 0)
            {
                continue;
            }
            
            // Find the net node for this pin
            int32_t net_idx = node_find_idx_by_name(nodes, transistor.pin_nets[pin]);
            if (net_idx < 0) continue;
            
            const node &net_node = nodes[net_idx];
            int32_t net_x = scale * net_node.x;
            int32_t net_y = scale * net_node.y;
            
            // Calculate pin position on the symbol
            int32_t pin_x, pin_y;
            if (transistor.comp_type == component_type::M)
            {
                // MOSFET pins: D(48,0), G(0,80), S(48,96)
                if (pin == 0) { pin_x = sym_x + 48; pin_y = sym_y + 0; }       // Drain
                else if (pin == 1) { pin_x = sym_x + 0; pin_y = sym_y + 80; }  // Gate
                else { pin_x = sym_x + 48; pin_y = sym_y + 96; }               // Source
            }
            else
            {
                // BJT pins: C(64,0), B(0,48), E(64,96)
                if (pin == 0) { pin_x = sym_x + 64; pin_y = sym_y + 0; }       // Collector
                else if (pin == 1) { pin_x = sym_x + 0; pin_y = sym_y + 48; }  // Base
                else { pin_x = sym_x + 64; pin_y = sym_y + 96; }               // Emitter
            }
            
            // Draw orthogonal wire from pin to net node
            write_orthogonal_wire(f, pin_x, pin_y, net_x, net_y);
        }
    }
}

