#include "include/souistd.h"
#include "helper/STimer.h"
#include "helper/STimerGenerator.h"

SNSBEGIN
STimer::STimer(IEvtSlot *pSlot):m_uTimerId(0)
{
	m_evtSlot.Attach(pSlot->Clone());
}

STimer::~STimer(void)
{
	KillTimer();
}

UINT STimer::StartTimer(THIS_ int nElapse,BOOL bRepeat)
{
	m_uTimerId = STimerGenerator::getSingletonPtr()->SetTimer(m_evtSlot,nElapse,bRepeat);
	return m_uTimerId;
}

void STimer::KillTimer(THIS)
{
	if(m_uTimerId){
		STimerGenerator::getSingletonPtr()->ClearTimer(m_uTimerId);
		m_uTimerId = 0;
	}
}

SNSEND