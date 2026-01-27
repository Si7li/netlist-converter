#ifndef LINE_VIEW_HPP
#define LINE_VIEW_HPP

#include "component_type.hpp"

struct line_view
{
    line_view();
    line_view(const char *in_p0, const char *in_p1, const char *in_val, const char *in_name, component_type in_type);

    char p0[7];
    char padp0;
    char p1[7];
    char padp1;
    char p2[7];      // Third pin (for transistors)
    char padp2;
    char p3[7];      // Fourth pin (for MOSFETs)
    char padp3;
    char val[8];
    char model[16];  // Model name for transistors and diodes
    char name[7];
    component_type comp_type;
    uint8_t n_pins;  // Number of pins (2 for most, 3 for BJT, 4 for MOSFET)
};

#endif /* LINE_VIEW_HPP */

