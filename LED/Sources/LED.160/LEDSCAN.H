/*
 *   LEDSCAN2.H      Scancodes der Tastatur                     
 *                   fÅr evnt_keybd()					 
 *                   eigenes LED-Format
 *
 *   Normal:     a = nur ASCII	 		(x61)
 *   Shift:      A = nur ASCII			(x41)
 *
 *   CTRL:      ^A = nur ASCII			(x01)
 *   ALT:       #A = ASCII in Highbyte 	(x6101)	    
 *
 *  	Sondertasten mit low 00:  wie bisher
 */
								
#define KEY_A               0x41
#define KEY_a               0x61

#define KEY_J               0x4a
#define KEY_j               0x6a

#define KEY_N               0x4e
#define KEY_n               0x6e

#define KEY_P               0x50
#define KEY_p               0x70

#define KEY_Q								0x51
#define KEY_q								0x71

#define KEY_R								0x52
#define KEY_r								0x72

#define KEY_T								0x54
#define KEY_t								0x74

#define KEY_W								0x57
#define KEY_w								0x77

#define KEY_X								0x58
#define KEY_x								0x78

#define DOPPKREUZ           0x23
#define STAR                0x2a
#define PLUS                0x2b
#define MINUS               0x2d

#define HELPKEY             0x6200
#define SHFT_HELPKEY				0x6280
#define CNTRL_HELPKEY				0xa700

#define ESC                 0x1b
#define TAB                 0x0f09
#define SPACE               0x20
#define RETURN              0x1c0d
#define ENTER               0x720d
#define BACKSPACE           0x0e08
#define DELETE              0x537f
#define INSERT              0x5200
#define HOME                0x4700
#define CUR_UP              0x4800
#define CUR_LEFT            0x4b00
#define CUR_RIGHT           0x4d00
#define CUR_DOWN            0x5000
#define UNDO                0x6100
#define CNTRL_SPACE					0x3900
#define ALT_RETURN					0x4900

#define SHFT_GROESSER       0x3e
#define GROESSER						0x3c

#define SHFT_INS            0x5230
#define SHFT_DEL						0x53ff
#define SHFT_HOME           0x4737
#define SHFT_CU             0x4838
#define SHFT_CL             0x4b34
#define SHFT_CR             0x4d36
#define SHFT_CD             0x5032

#define ALT_A               0x6101
#define SHIFT_ALT_A         0x4101
#define ALT_B               0x6201
#define ALT_C               0x6301
#define ALT_D               0x6401
#define SHIFT_ALT_D         0x4401
#define ALT_E               0x6501
#define SHIFT_ALT_E         0x4501
#define ALT_F               0x6601
#define ALT_G               0x6701
#define ALT_H               0x6801
#define SHIFT_ALT_H         0x4801
#define ALT_I               0x6901
#define ALT_J               0x6a01
#define ALT_K               0x6b01
#define ALT_L               0x6c01
#define ALT_M               0x6d01
#define ALT_N               0x6e01
#define ALT_O               0x6f01
#define ALT_P               0x7001
#define ALT_Q               0x7101
#define ALT_R               0x7201
#define ALT_S               0x7301
#define ALT_T               0x7401
#define ALT_U               0x7501
#define ALT_V               0x7601
#define ALT_W               0x7701
#define SHIFT_ALT_W         0x5701
#define ALT_X               0x7801
#define ALT_Y               0x7901
#define SHIFT_ALT_Y         0x5901
#define ALT_Z               0x7a01


#define ALT_1               0x3101
#define ALT_2               0x3201
#define ALT_3               0x3301
#define ALT_4               0x3401
#define ALT_5               0x3501
#define ALT_6               0x3601
#define ALT_7               0x3701
#define ALT_8               0x3801
#define ALT_9               0x3901
#define ALT_0               0x3001

#define CNTRL_A             0x01
#define CNTRL_B             0x02
#define CNTRL_C             0x03
#define CNTRL_D             0x04
#define CNTRL_E             0x05
#define CNTRL_F             0x06
#define CNTRL_G             0x07
#define CNTRL_H             0x08
#define CNTRL_I             0x09
#define CNTRL_J             0x0a
#define CNTRL_K             0x0b
#define CNTRL_L             0x0c
#define CNTRL_M             0x0d
#define CNTRL_N             0x0e
#define CNTRL_O             0x0f
#define CNTRL_P             0x10
#define CNTRL_Q             0x11
#define CNTRL_R             0x12
#define CNTRL_S             0x13
#define CNTRL_T             0x14
#define CNTRL_U             0x15
#define CNTRL_V             0x16
#define CNTRL_W             0x17
#define CNTRL_X             0x18
#define CNTRL_Y             0x19
#define CNTRL_Z             0x1a

#define SHIFT_CNTRL_A       0x0101
#define SHIFT_CNTRL_B       0x0102
#define SHIFT_CNTRL_C       0x0103
#define SHIFT_CNTRL_D       0x0104
#define SHIFT_CNTRL_E       0x0105
#define SHIFT_CNTRL_F       0x0106
#define SHIFT_CNTRL_G       0x0107
#define SHIFT_CNTRL_H       0x0108
#define SHIFT_CNTRL_I       0x0109
#define SHIFT_CNTRL_J       0x010a
#define SHIFT_CNTRL_K       0x010b
#define SHIFT_CNTRL_L       0x010c
#define SHIFT_CNTRL_M       0x010d
#define SHIFT_CNTRL_N       0x010e
#define SHIFT_CNTRL_O       0x010f
#define SHIFT_CNTRL_P       0x0110
#define SHIFT_CNTRL_Q       0x0111
#define SHIFT_CNTRL_R       0x0112
#define SHIFT_CNTRL_S       0x0113
#define SHIFT_CNTRL_T       0x0114
#define SHIFT_CNTRL_U       0x0115
#define SHIFT_CNTRL_V       0x0116
#define SHIFT_CNTRL_W       0x0117
#define SHIFT_CNTRL_X       0x0118
#define SHIFT_CNTRL_Y       0x0119
#define SHIFT_CNTRL_Z				0x011a

#define CNTRL_1             0x0211
#define CNTRL_2             0x0300
#define CNTRL_3             0x0413
#define CNTRL_4             0x0514
#define CNTRL_5             0x0615
#define CNTRL_6             0x071e
#define CNTRL_7             0x0817
#define CNTRL_8             0x0918
#define CNTRL_9             0x0a19
#define CNTRL_0             0x0b10
#define CNTRL_DEL           0x531f
#define CNTRL_RET           0x1c0a
#define CNTRL_CL            0x7300
#define CNTRL_CR            0x7400
#define CNTRL_CU            0x8d00
#define CNTRL_CD            0x9500

#define F1                  0x3b00
#define F2                  0x3c00
#define F3                  0x3d00
#define F4                  0x3e00
#define F5                  0x3f00
#define F6                  0x4000
#define F7                  0x4100
#define F8                  0x4200
#define F9                  0x4300
#define F10                 0x4400
#define SHFT_F1             0x5400
#define SHFT_F2             0x5500
#define SHFT_F3             0x5600
#define SHFT_F4             0x5700
#define SHFT_F5             0x5800
#define SHFT_F6             0x5900
#define SHFT_F7             0x5a00
#define SHFT_F8             0x5b00
#define SHFT_F9             0x5c00
#define SHFT_F10            0x5d00
#define ALT_F1              0x6800
#define ALT_F2              0x6900
#define ALT_F3              0x6a00
#define ALT_F4              0x6b00
#define ALT_F5              0x6c00
#define ALT_F6              0x6d00
#define ALT_F7              0x6e00
#define ALT_F8              0x6f00
#define ALT_F9              0x7000
#define ALT_F10             0x7100
#define CNTRL_F1            0x8000
#define CNTRL_F2            0x8100
#define CNTRL_F3            0x8200
#define CNTRL_F4            0x8300
#define CNTRL_F5            0x8400
#define CNTRL_F6            0x8500
#define CNTRL_F7            0x8600
#define CNTRL_F8            0x8700
#define CNTRL_F9            0x8800
#define CNTRL_F10           0x8900

#define ZEHN_PLUS           0x4e2b
#define ZEHN_MINUS          0x4a2d
#define ZEHN_STAR           0x662a
#define ZEHN_SLASH          0x652f
#define ZEHN_OPKL           0x6328
#define ZEHN_CLKL           0x6429
#define ZEHN_CT_OPKL        0x6308
#define ZEHN_CT_CLKL        0x6409

#define ZEHN_0				0x7030
#define ZEHN_1				0x6d31
#define ZEHN_2				0x6e32
#define ZEHN_3				0x6f33
#define ZEHN_4				0x6a34
#define ZEHN_5				0x6b35
#define ZEHN_6				0x6c36
#define ZEHN_7				0x6737
#define ZEHN_8				0x6838
#define ZEHN_9				0x6939

#define ZEHN_ALT_0          0x9d30
#define ZEHN_ALT_1          0x9a31
#define ZEHN_ALT_2          0x9b32
#define ZEHN_ALT_3          0x9c33
#define ZEHN_ALT_4          0x9734
#define ZEHN_ALT_5          0x9835
#define ZEHN_ALT_6          0x9936
#define ZEHN_ALT_7          0x9437
#define ZEHN_ALT_8          0x9538
#define ZEHN_ALT_9          0x9639

#define ZEHN_CT_0           0x7010
#define ZEHN_CT_1           0x6D11
#define ZEHN_CT_2           0x6E00
#define ZEHN_CT_3           0x6F13
#define ZEHN_CT_4           0x6A14
#define ZEHN_CT_5           0x6B15
#define ZEHN_CT_6           0x6C1e
#define ZEHN_CT_7           0x6717
#define ZEHN_CT_8           0x6818
#define ZEHN_CT_9           0x6919
#define ZEHN_CT_PT          0x710E

#define ZEHN_SHFT_0         0x70B0
#define ZEHN_SHFT_1         0x6DB1
#define ZEHN_SHFT_2         0x6EB2
#define ZEHN_SHFT_3         0x6FB3
#define ZEHN_SHFT_4         0x6AB4
#define ZEHN_SHFT_5         0x6BB5
#define ZEHN_SHFT_6         0x6CB6
#define ZEHN_SHFT_7         0x67B7
#define ZEHN_SHFT_8         0x68B8
#define ZEHN_SHFT_9         0x69B9
#define ZEHN_SHFT_PT        0x71AE

/************************************************************************/
