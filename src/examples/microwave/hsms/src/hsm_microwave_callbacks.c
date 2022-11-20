#include <QuarkTS.h>
#include <system_signals.h>

//
// Callbacks
//
qSM_Status_t microwave_top_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("TOP|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("TOP|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("TOP|S");
      break;
  }

  return RetVal;
}

qSM_Status_t microwave_door_closed_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("DOOR_CLOSED|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("DOOR_CLOSED|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("DOOR_CLOSED|S");
      break;
  }

  return RetVal;
}

qSM_Status_t microwave_heating_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("HEATING|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("HEATING|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("HEATING|S");
      break;
  }

  return RetVal;
}

qSM_Status_t microwave_off_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("OFF|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("OFF|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("OFF|S");
      break;
  }

  return RetVal;
}

qSM_Status_t microwave_toasting_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("TOASTING|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("TOASTING|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("TOASTING|S");
      break;
  }

  return RetVal;
}

qSM_Status_t microwave_baking_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("BAKING|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("BAKING|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("BAKING|S");
      break;
  }

  return RetVal;
}

qSM_Status_t microwave_door_opened_state_callback (qSM_Handler_t h)
{
  qSM_Status_t RetVal = qSM_STATUS_EXIT_SUCCESS;

  switch (h->Signal.type) {
    case QSM_SIGNAL_TYPE_ENTRY :
      qDebug_Message("DOOR_OPENED|E");
      break;
    case QSM_SIGNAL_TYPE_EXIT :
      qDebug_Message("DOOR_OPENED|X");
      break;
    case QSM_SIGNAL_TYPE_START :
      qDebug_Message("DOOR_OPENED|S");
      break;
  }

  return RetVal;
}
