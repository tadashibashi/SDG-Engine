/* =============================================================================
 * VM
 * 
 * 
 * ===========================================================================*/
#include "VM.h"
#include <cassert>

namespace SDG
{
    VM::VM()
    {
    }

    VM::~VM()
    {
    }

    void VM::Push(unsigned int val)
    {
        assert(stack_.size() < MAX_STACK);
        stack_.push(val);
    }

    unsigned VM::Pop()
    {
        assert(!stack_.empty());
        unsigned val = stack_.top();
        stack_.pop();
        return val;
    }
}