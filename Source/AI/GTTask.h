#ifndef GTTASK_H_
#define GTTASK_H_

/* ====== INCLUDES ====== */
#include <Types.h>

/* ====== DEFINES ====== */
enum eGTTaskStatus
{
    GTT_DONE = 1,
    GTT_INPROCESS = 2,

    GTT_STATUS_END
};

enum eGTTaskID
{
    GTT_TASK_START = GTT_STATUS_END,

    GTT_NONE = 0,
    GTT_GOTO = GTT_TASK_START
};

/* ====== STRUCTURES ====== */
class Actor;

// Tasks send commands to Actors
class GT_Task
{
protected:
    s32 m_id;
    s32 m_status;
    Actor* m_pActor;
public:
    GT_Task(Actor* pActor, s32 id) : m_pActor(pActor), m_id(id), m_status(GTT_INPROCESS) {}
    virtual ~GT_Task() {}

    s32 GetID() const { return m_id; }
    s32 GetStatus() const { return m_status; }

    virtual void Handle() = 0;
};

#endif // GTTASK_H_