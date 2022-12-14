#include "DebugLogManager.h"
#include "Actor.h"
#include "GTUnit.h"

/* ====== INCLUDES ====== */
#include "GotoTask.h"

/* ====== DEFINES ====== */
#define ERROR_MULTIPLIER 10.0f

/* ====== METHODS ====== */
void GotoTask::Handle()
{
    if (m_status != GTT_INPROCESS)
        return;

    // Get position and compute error
    const Vector2& vPosition = m_pActor->m_vPosition;
    Vector2 vError = { m_pActor->m_vSpeed.x * ERROR_MULTIPLIER,
                       m_pActor->m_vSpeed.y * ERROR_MULTIPLIER };

    // X
    if (!m_bCompletedX)
    {
        if (m_vDestination.x < vPosition.x - vError.x)
            m_pActor->PushCommand(GTC_MOVE_LEFT);
        else if (m_vDestination.x > vPosition.x + vError.x)
            m_pActor->PushCommand(GTC_MOVE_RIGHT);
        else
            m_bCompletedX = true;
    }

    // Y
    if (!m_bCompletedY)
    {
        if (m_vDestination.y < vPosition.y - vError.y)
            m_pActor->PushCommand(GTC_MOVE_UP);
        else if (m_vDestination.y > vPosition.y + vError.y)
            m_pActor->PushCommand(GTC_MOVE_DOWN);
        else
            m_bCompletedY = true;
    }

    // Done
    if (m_bCompletedX && m_bCompletedY)
        m_status = GTT_DONE;
}
