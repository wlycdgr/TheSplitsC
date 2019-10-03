#ifndef SPLITSLIB_T_Time_H
#define SPLITSLIB_T_Time_H

void splitslib_t_AdvanceTimer(int id);
void splitslib_t_FreeTimer(int id);
bool splitslib_t_HasTimerGoneOff(int id);
int splitslib_t_InitTimer(float targetTimeInSecs);
void splitslib_t_ResetTimer(int id);

#endif