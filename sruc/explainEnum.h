#pragma once
// 状态
typedef enum {
    OUT_PP,
    OUT_OD,
    IN_FALOTING,
    IPU,
    IPD,
    AF_PP,
    AF_OD,
    AIN,
}IO;

typedef enum {
    Rising,
    Falling,
    Ris_Fall,
}EXTI;

typedef enum {
    Unknown,
    GPIO,
    GPIOsignle,
    TIM,
    G,
    M,
}dType;

typedef enum {
    NONE,
    GPIOINIT,
    GPIOINPT,
    GLOBAL,
    MAIN,
    TIMINIT
}CodeType;