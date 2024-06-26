#if !defined(EVAL_H)
#define EVAL_H

#include "core.h"

inline Cell f_val(double val) {
    Cell c;
    c.floatVal = val;
    c.type = Type::FLOAT;
    return c;
}

inline Cell i_val(int val) {
    Cell c;
    c.intVal = val;
    c.type = Type::INT;
    return c;
}

inline Cell s_val(const string& val) {
    return Cell(val);
}

#endif // EVAL_H
