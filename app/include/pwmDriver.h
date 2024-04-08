#ifndef _PWMDRIVER_H_
#define _PWMDRIVER_H_

//Creates thread for buzzer on Hit
void BuzzerHitThreadCreate(void);

//Join Thread for buzzer on Hit
void BuzzerHitThreadJoin(void);

//Creates thread for buzzer on Miss
void BuzzerMissThreadCreate(void);

//Join Thread for buzzer on Miss 
void BuzzerMissThreadJoin(void);

#endif