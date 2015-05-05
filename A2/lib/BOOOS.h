/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 */

#ifndef BOOOS_H_
#define BOOOS_H_

namespace BOOOS {

/* Configuration Space */

class BOOOS {
public:

	enum SchedulerType {
		SCHED_FCFS,
		SCHED_PRIORITY
	};
	
    static BOOOS * __booos ;
    static BOOOS::SchedulerType SCHED_POLICY; // ou outro escalonador. Ajustem como necessário
    static bool SCHED_PREEMPT;  // pode ser preemptivo ou não
    static bool SCHED_AGING;  // apenas alguns escalonadores usam aging. Ajustem como necessário

	BOOOS(bool verbose = true);
	~BOOOS();

	static const int BOOOS_VERSION = 0;

	void panic();

private:
	bool _verbose;
};

} /* namespace BOOOS */

#endif /* BOOOS_H_ */
