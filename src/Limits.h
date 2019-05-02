#ifndef LIMITS__HEADER
#define LIMITS__HEADER

#define RETURN_IF_VALID(VAL, MIN, MAX, RET) return (VAL < MIN || VAL > MAX) ? NULL : RET

#define MONSTER_NUM_MIN    0
#define MONSTER_NUM_MAX    6
#define MONSTER_TYPE_MIN   0
#define MONSTER_TYPE_MAX   5
#define LEVEL_MIN          1
#define LEVEL_MAX          102

#define PALETTE_MIN         0
#define PALETTE_MAX         7
#define X32_MIN            32
#define X32_MAX           (28*16)
#define Y32_MIN            16
#define Y32_MAX           (23*16)
#define X16_MIN            32
#define X16_MAX           (29*16)
#define Y16_MIN            16
#define Y16_MAX           (22*16)
#define TOPBOT_MIN          0
#define TOPBOT_MAX          8
#define BREAK_SPEED_MIN     0
#define BREAK_SPEED_MAX   130
#define MONSTER_SPEED_MIN   0
#define MONSTER_SPEED_MAX  40
#define BUBBLE_SPEED_MIN    0
#define BUBBLE_SPEED_MAX   40
#define TIMER_MIN           0
#define TIMER_MAX          60
#define BARON_TIME_MIN      0
#define BARON_TIME_MAX    255
#define BARON_SPEED_MIN     0
#define BARON_SPEED_MAX   255
#define BUBBLE_RATE_MIN     0
#define BUBBLE_RATE_MAX    16
#define MONSTER_DROP_MIN  0
#define MONSTER_DROP_MAX  255
#define MONSTER_DIR_MIN     0
#define MONSTER_DIR_MAX     1

#define BLOCK_X_MIN        0
#define BLOCK_X_MAX        31
#define BLOCK_Y_MIN        0
#define BLOCK_Y_MAX        25
#define PIXEL_X_MIN        0
#define PIXEL_X_MAX        ((32*16)-1)
#define PIXEL_Y_MIN        0
#define PIXEL_Y_MAX        ((26*16)-1)
#define REGION16_X_MIN     (PIXEL_X_MIN+32)
#define REGION16_X_MAX     (PIXEL_X_MAX-47)
#define REGION16_Y_MIN     (PIXEL_Y_MIN+16)
#define REGION16_Y_MAX     (PIXEL_Y_MAX-31)
#define REGION32_X_MIN     (PIXEL_X_MIN+32)
#define REGION32_X_MAX     (PIXEL_X_MAX-63)
#define REGION32_Y_MIN     (PIXEL_Y_MIN+16)
#define REGION32_Y_MAX     (PIXEL_Y_MAX-47)

#endif
