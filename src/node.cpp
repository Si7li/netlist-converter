#include "node.hpp"
#include "logger.hpp"
#include <string.h>

node::node() : val{}, name{}, pin_nets{}, n_pins(0) {}

node::node(const char *in_val, const char *in_name, component_type in_type)
    : node()
{
    strncpy(&val[0], &in_val[0], sizeof(val) - 1);
    strncpy(&name[0], &in_name[0], sizeof(name) - 1);
    comp_type = in_type;
}

namespace
{
bool node_is_gnd(const char *name)
{
    char gnd_name[] = "0";
    return (0 == strncmp(name, gnd_name, sizeof(gnd_name)));
}
}

void node_extract(std::vector<node> &nodes, const line_view &view)
{
    bool add_name = true;
    bool add_p0 = true;
    bool add_p1 = true;
    bool add_p2 = true;
    bool add_p3 = true;

    for (node &n : nodes)
    {
        if (0 == strncmp(view.name, n.name, sizeof(n.name)))
        {
            LOG_WARN("Node with the name %s already exists, val=%s, type=%d", n.name,
                    n.val, static_cast<int32_t>(n.comp_type));
            add_name = false;
        }
        if (0 == strncmp(view.p0, n.name, sizeof(n.name)))
        {
            LOG_WARN("Node p0 with the name %s already exists, val=%s, type=%d", n.name,
                    n.val, static_cast<int32_t>(n.comp_type));
            add_p0 = false;
        }
        if (0 == strncmp(view.p1, n.name, sizeof(n.name)))
        {
            LOG_WARN("Node p1 with the name %s already exists, val=%s, type=%d", n.name,
                    n.val, static_cast<int32_t>(n.comp_type));
            add_p1 = false;
        }
        if ((view.n_pins >= 3) && (0 == strncmp(view.p2, n.name, sizeof(n.name))))
        {
            LOG_WARN("Node p2 with the name %s already exists, val=%s, type=%d", n.name,
                    n.val, static_cast<int32_t>(n.comp_type));
            add_p2 = false;
        }
        if ((view.n_pins >= 4) && (0 == strncmp(view.p3, n.name, sizeof(n.name))))
        {
            LOG_WARN("Node p3 with the name %s already exists, val=%s, type=%d", n.name,
                    n.val, static_cast<int32_t>(n.comp_type));
            add_p3 = false;
        }
    }

    if (add_name)
    {
        // For transistors and diodes, use model as val
        const char *val_to_use = (view.comp_type == component_type::Q || 
                                   view.comp_type == component_type::M || 
                                   view.comp_type == component_type::D) ? view.model : view.val;
        nodes.emplace_back(val_to_use, view.name, view.comp_type);
        
        // Store pin-to-net mapping for transistors
        node &component_node = nodes.back();
        component_node.n_pins = view.n_pins;
        if (view.n_pins >= 2) {
            strncpy(component_node.pin_nets[0], view.p0, sizeof(component_node.pin_nets[0]) - 1);
            strncpy(component_node.pin_nets[1], view.p1, sizeof(component_node.pin_nets[1]) - 1);
        }
        if (view.n_pins >= 3) {
            strncpy(component_node.pin_nets[2], view.p2, sizeof(component_node.pin_nets[2]) - 1);
        }
        if (view.n_pins >= 4) {
            strncpy(component_node.pin_nets[3], view.p3, sizeof(component_node.pin_nets[3]) - 1);
        }
        
        if (add_p0)
        {
            component_type comp_type = node_is_gnd(view.p0) ? component_type::Ground : component_type::DotPoint;
            nodes.emplace_back(val_to_use, view.p0, comp_type);
        }
        if (add_p1)
        {
            component_type comp_type = node_is_gnd(view.p1) ? component_type::Ground : component_type::DotPoint;
            nodes.emplace_back(val_to_use, view.p1, comp_type);
        }
        if ((view.n_pins >= 3) && add_p2)
        {
            component_type comp_type = node_is_gnd(view.p2) ? component_type::Ground : component_type::DotPoint;
            nodes.emplace_back(val_to_use, view.p2, comp_type);
        }
        if ((view.n_pins >= 4) && add_p3)
        {
            component_type comp_type = node_is_gnd(view.p3) ? component_type::Ground : component_type::DotPoint;
            nodes.emplace_back(val_to_use, view.p3, comp_type);
        }
    }
}

int32_t node_find_idx_by_name(const std::vector<node> &nodes, const char *name)
{
    int32_t idx = -1;
    for (int32_t i = 0; i < nodes.size(); ++i)
    {
        if (0 == strncmp(nodes[i].name, name, sizeof(nodes[i].name)))
        {
            idx = i;
            break;
        }
    }

    return idx;
}

