#include <QuarkTS.h>
#include <system_signals.h>

//
// Callbacks
//
qSM_Status_t pse_sa_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sa1_state_callback (qSM_Handler_t h)
{
    qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA1|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA1|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA1|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qBool_t pse_sa1_state_guard ( qSM_Handler_t h )
{
  qBool_t retValue = qTrue;

  qDebug_Message("SA1|GUARD");

  return retValue;
}

qSM_Status_t pse_sa11_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA11|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA11|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA11|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sa12_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA12|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA12|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA12|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sa2_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA2|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA2|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA2|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sa21_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA21|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA21|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA21|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sa22_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SA22|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SA22|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SA22|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb1_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB1|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB1|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB1|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb11_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB11|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB11|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB11|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb12_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB12|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB12|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB12|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb2_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB2|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB2|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB2|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb21_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB21|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB21|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB21|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}

qSM_Status_t pse_sb22_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal) {
    case QSM_SIGNAL_ENTRY :
      qDebug_Message("SB22|E");
      break;
    case QSM_SIGNAL_EXIT :
      qDebug_Message("SB22|X");
      break;
    case QSM_SIGNAL_START :
      qDebug_Message("SB22|S");
      break;
    default:
      RetVal = qSM_STATUS_EXIT_FAILURE;
      break;
  }

  return RetVal;
}
