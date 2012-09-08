#include "agg_color_cmyk.h"

const cmyk cmyk::black   = cmyk(0.0, 0.0, 0.0, 1.0);
const cmyk cmyk::white   = cmyk(0.0, 0.0, 0.0, 0.0);
const cmyk cmyk::cyan    = cmyk(1.0, 0.0, 0.0, 0.0);
const cmyk cmyk::magenta = cmyk(0.0, 1.0, 0.0, 0.0);
const cmyk cmyk::yellow  = cmyk(0.0, 0.0, 1.0, 0.0);

const cmyka8 cmyka8::black   = cmyk::black;
const cmyka8 cmyka8::white   = cmyk::white;
const cmyka8 cmyka8::cyan    = cmyk::cyan;
const cmyka8 cmyka8::yellow  = cmyk::yellow;
const cmyka8 cmyka8::magenta = cmyk::magenta;

