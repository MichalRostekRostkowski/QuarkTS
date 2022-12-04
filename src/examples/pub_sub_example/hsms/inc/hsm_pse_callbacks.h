#ifndef _HSM_PSE_H_
#define _HSM_PSE_H_

//
// Callbacks
//
qSM_Status_t pse_sa_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sa1_state_callback (qSM_Handler_t h);
qBool_t pse_sa1_state_guard ( qSM_Handler_t h );
qSM_Status_t pse_sa11_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sa12_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sa2_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sa21_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sa22_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb1_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb11_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb12_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb2_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb21_state_callback (qSM_Handler_t h);
qSM_Status_t pse_sb22_state_callback (qSM_Handler_t h);

#endif // _HSM_PSE_H_