/*
 * semaphore.h
 *
 *  Created on: Dec 6, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/*
 * semaphore.h
 */

#define SEMA_OPEN  	0
#define SEMA_CLOSED	1

void semaWait(unsigned int *semaphore);
void semaPost(unsigned int *semaphore);

#endif /* SEMAPHORE_H_ */
