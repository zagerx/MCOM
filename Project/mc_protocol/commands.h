#ifndef COMMANDS_H
#define COMMANDS_H

#define M_CMD                        (0x01)
#define M_SET_SPEED                  (0x0101)
#define M_SET_POS                    (0x0102)
#define M_SET_START                  (0x0103)
#define M_SET_STOP                   (0x0104)
#define M_SET_NormalM                (0x0105)
#define M_SET_SpeedM                 (0x0106)
#define M_GET_MotorInfo              (0x0107)
#define M_GET_CtrlParaseInfo               (0x0108)
#define M_SET_PIDParam               (0x0109)
#define M_SET_PIDTarge               (0x010A)

#define S_CMD                        (0x02)
#define S_HeartP                     (0x0200)
#define S_SMInfo                     (0x0201)
#define S_PCBAInfo                   (0x0202)
#define S_MotorInfo                  (0x0203)
#define S_MotorSpeed                 (0x0204)




#endif // COMMANDS_H