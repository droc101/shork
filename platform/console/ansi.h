//
// Created by droc101 on 4/9/25.
//

#ifndef ANSI_H
#define ANSI_H

#define ANSI_HIDE_CURSOR "\033[?25l"
#define ANSI_SHOW_CURSOR "\033[?25h"
#define ANSI_RESET_COLORS "\033[0m"
#define ANSI_CLEAR_SCREEN "\033[H\033[J"
#define ANSI_MOVE_CURSOR_TOP_LEFT "\033[H"

#define ANSI_FG_COLOR(R, G, B) "\033[38;2;" #R ";" #G ";" #B "m"
#define ANSI_BG_COLOR(R, G, B) "\033[48;2;" #R ";" #G ";" #B "m"
#define ANSI_FG_COLOR_FMT "\033[38;2;%d;%d;%dm"
#define ANSI_BG_COLOR_FMT "\033[48;2;%d;%d;%dm"

#endif //ANSI_H
