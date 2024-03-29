
// Identiferの種類
typedef enum kindT {
	varId, funcId, parId, constId
}KindT;

// 変数、パラメタ、関数のアドレスの型
typedef struct relAddr {
  int level;
  int addr;
} RelAddr;

// ブロックの始まり(最初の変数の番地)で呼ぶ
void blockBegin(int firstAddr);

// ブロックの終わりで呼ばれる
void blockEnd();

// 現ブロックのレベルを返す
int bLevel();

// 現ブロックの関数のパラメタ数を返す
int fPars();

// 名前表に関数名と先頭番地を登録
int enterTfunc(char *id, int v);

// 名前表に変数名を登録
int enterTvar(char *id);

// 名前表にパラメタ名を登録
int enterTpar(char *id);

// 名前表に定数名とその値を登録
int enterTconst(char *id, int v);

// パラメタ宣言部の最後で呼ばれる
void endpar();

// 名前表[ti]の値(関数名の先頭番地)の変更
void changeV(int ti, int newVal);

// 名前idの名前表の位置を返す
int searchT(char *id, KindT k);

// 名前表[i]の種類を返す
KindT kindT(int i);

// 名前表[ti]のアドレスを返す
RelAddr relAddr(int ti);

// 名前表[ti]のvalueを返す
int val(int ti);

// 名前表[ti]の関数パラメタ数を返す
int pars(int ti);

// そのブロックで実行時に必要とするメモリ容量
int frameL();