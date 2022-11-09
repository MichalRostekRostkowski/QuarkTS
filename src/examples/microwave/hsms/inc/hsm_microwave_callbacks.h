#ifndef _HSM_MICROWAVE_H_
#define _HSM_MICROWAVE_H_

qSM_Status_t microwave_top_state_callback (qSM_Handler_t h);
qSM_Status_t microwave_door_closed_state_callback (qSM_Handler_t h);
qSM_Status_t microwave_heating_state_callback (qSM_Handler_t h);
qSM_Status_t microwave_off_state_callback (qSM_Handler_t h);
qSM_Status_t microwave_toasting_state_callback (qSM_Handler_t h);
qSM_Status_t microwave_baking_state_callback (qSM_Handler_t h);
qSM_Status_t microwave_door_opened_state_callback (qSM_Handler_t h);

#endif // _HSM_MICROWAVE_H_