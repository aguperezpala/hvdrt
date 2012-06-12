/*
 * FrameBuffer.h
 *
 *  Created on: 10/06/2012
 *      Author: agustin
 *
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <deque>

#include "Frame.h"
#include "DebugUtil.h"
#include "MultiValueMutex.h"



class FrameBuffer {
public:
	FrameBuffer()
	{

	}
	~FrameBuffer()
	{
		freeAll();
	}

	/**
	 * Add a new available frame
	 */
	inline void addNewAvailable(Frame *f);

	/**
	 * Returns a new available frame, or blocks if there are no more available
	 * frames
	 */
	inline Frame *getFrame(void);

	/**
	 * Create N frames
	 */
	inline void createFrames(int n);

	/**
	 * Free all the frames
	 */
	inline void freeAll(void);

	/**
	 * Return the size
	 */
	inline int size(void) const;

private:
	std::deque<Frame *>		mQueue;
	MultiValueMutex			mSemaphore;

};


/**
 * Add a new available frame
 */
inline void FrameBuffer::addNewAvailable(Frame *f)
{
	mQueue.push_back(f);
	mSemaphore.unlock();
}

/**
 * Returns a new available frame, or 0 if there are not any free frame
 */
inline Frame *FrameBuffer::getFrame(void)
{
	mSemaphore.lock();
	if(mQueue.empty()) return 0;
	Frame *r = mQueue.front();
	mQueue.pop_front();
	return r;
}

/**
 * Create N frames
 */
inline void FrameBuffer::createFrames(int n)
{
	ASSERT(n > 0);
	for(int i = 0; i < n; ++i){
		Frame *f = new Frame;
		addNewAvailable(f);
	}
}

/**
 * Free all the frames
 */
inline void FrameBuffer::freeAll(void)
{
	Frame *f = getFrame();
	while(f){
		delete f;
		f = getFrame();
	}

}

/**
 * Return the size
 */
inline int FrameBuffer::size(void) const
{
	return mQueue.size();
}

#endif /* FRAMEBUFFER_H_ */
