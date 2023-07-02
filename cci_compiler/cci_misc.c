#include "cci.h"

// 許容最大エラー個数
#define MAX_ERR 10

// 漢字第1バイト目なら漢字コードのバイト数、そうでなければ0を返す
int is_kanji(int ch) {
  int uch = (unsigned char)ch;
  static int kanji_mode = -1;

  if (kanji_mode == -1)
    kanji_mode = get_kanji_mode();
  switch (kanji_mode) {
    case 'S':
      return (0x81<=uch && uch <= 0x9f) || (0xe0 <= uch && uch <= 0xfc) ? 2 : 0;
    case 'E':
      return uch == 0x8f ? 3 : (uch >= 0x80 ? 2 : 0);
    case 'U':
      return uch >= 0xe0 ? 3 : (uch >= 0xc0 ? 2 : 0);
  }
}

// 漢字モード判定：'S':SHIFT-JIS 'E':EUC-JP 'U':UTF-8 0:それ以外を返す
int get_kanji_mode() {
  int ret = 0;
  // 漢字コードを調べるためのサンプル文字
  static unsigned char kan[] = "漢";

  // SHIT-JIS
  if (kan[0] == 0x8a && kan[1] == 0xbf)
    ret = 'S';
  // EUC-JP
  else if (kan[0] == 0xb4 && kan[1] == 0xc1)
    ret = 'E';
  // UTF-8
  else if (kan[0] == 0xe6 && kan[1] == 0xbc && kan[2] == 0xa2)
    ret = 'U';
  return ret;
}

// エラー表示
void err_ss(char *s1, char *s2) {
  extern int err_ct;
  static int olderr_lineno = -1;

  if (olderr_lineno == get_lineno());
}