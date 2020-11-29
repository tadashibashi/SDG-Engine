/* =============================================================================
 * Chronogram
 * 
 * 
 * ===========================================================================*/
#include "Chronogram.h"
#include <Engine/Time/GameTime.h>

namespace SDG
{
    unsigned long Chronogram::CheckTicks(unsigned long timerIndex) const
    {
        return time_->GetTotalTicks() - timers_.at(timerIndex);
    }

    double Chronogram::CheckSeconds(unsigned long timerIndex) const
    {
        return CheckTicks(timerIndex) * 0.001;
    }

    void Chronogram::StartTimer(unsigned long timerIndex)
    {
        timers_[timerIndex] = time_->GetTotalTicks();
    }


}