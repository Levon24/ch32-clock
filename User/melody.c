#include "melody.h"

// Мобильник (из Бумера)
const _melody_t melody00 = {
  180, {
    8,	E|O2,		16,	G|O2,		16,	P|O2,		8,	G|O2,		16,	E|O2,		16,	P|O2,		8,	A|O2,		8,	G|O2,
    8,	A|O2,		8,	G|O2,		8,	A|O2,		8,	G|O2,		8,	A|O2,		8,	G|O2,		8,	A|O2,		16,	B|O2
  }
};

// Белые розы
const _melody_t melody01 = {
  120, {
    8,	B|O1,		8,	B|O1,		8,	B|O1,		16,	B|O1,		16,	F_|O1,	8,	P|O2,		8,	B|O1,		8,	B|O1,
    8,	B|O1,		16,	B|O1,		16,	F_|O1,	8,	P|O2,		16,	B|O1,		16,	G|O1,		8,	F_|O1,	8,	G|O1,
    8,	F_|O1,	16,	G|O1,		8,	P|O2,		8,	P|O2,		8,	P|O2,		8,	A|O1,		8,	A|O1,		8,	A|O1,
    16,	A|O1,		16,	E|O1,		8,	P|O2,		8,	A|O1,		8,	A|O1,		8,	A|O1,		16,	A|O1,		16,	E|O1,
    8,	P|O2,		16,	A|O1,		16,	F|O1,		8,	E|O1,		8,	F|O1,		8,	E|O1,		16,	F|O1
  }
};

// ДДТ: Оcень
const _melody_t melody02 = {
  120, {
    8,	E|O2,		4,	C|O2,		12,	B|O1,		8,	A|O1,		12,	E|O2,		12,	B|O1,		4,	B|O1,		8,	C|O2,
    12,	B|O1,		48,	A|O1,		8,	A|O1,		4,	A|O1,		12,	A|O1,		8,	A|O1,		12,	A|O1,		8,	A|O1,
    4,	C_|O2,	12,	E|O2,		12,	G|O2,		48,	F|O2,		12,	D|O2,		4,	D|O2,		8,	D|O2,		8,	D|O2,
    8,	G|O2,		8,	F|O2,		12,	E|O2,		12,	D|O2,		8,	E|O2,		8,	E|O2,		4,	E|O2,		8,	D|O2,
    12,	C|O2,		16,	A|O1,		8,	P|O2,		8,	B|O1,		8,	B|O1,		8,	G|O2,		8,	F|O2,		8,	E|O1,
    4,	E|O1,		8,	E|O2,		12,	C|O2,		12,	B|O1,		32,	A|O1
  }
};

// Europe: Final countdown
const _melody_t melody03 = {
  120, {
    4,	C|O2,		4,	A_|O1,	16,	C|O2,		32,	F|O1,		4,	C_|O2,	4,	C|O2,		8,	C_|O2,	8,	C|O2,
    32,	A_|O1,	4,	C_|O2,	4,	C|O2,		16,	C_|O2,	32,	F|O1,		4,	A_|O1,	4,	G_|O1,	8,	A_|O1,
    8,	G_|O1,	8,	G|O1,		8,	A_|O1,	32,	G_|O1,	4,	G|O1,		4,	G_|O1,	32,	A_|O1,	4,	G_|O1,
    4,	A_|O1,	8,	C|O2,		8,	A_|O1,	8,	G_|O1,	8,	G|O1,		16,	F|O1,		16,	C_|O2,	64,	C|O2,
    4,	C|O2,		4,	C_|O2,	4,	C|O2,		4,	A_|O1,	64,	C|O2
  }
};

// AQUA
const _melody_t melody04 = {
  120, {
    8,	G_|O2,	8,	E|O2,		8,	G_|O2,	8,	C_|O3,	16,	A|O2,		16,	P|O2,		8,	F_|O2,	8,	D_|O2,
    8,	F_|O2,	8,	B|O2,		16,	G_|O2,	8,	F_|O2,	8,	E|O2,		16,	P|O2,		8,	E|O2,		8,	C_|O2,
    16,	F_|O2,	16,	C_|O2,	16,	P|O2,		8,	F_|O2,	8,	E|O2,		16,	G_|O2,	16,	F_|O2
  }
};

// ABBA: MoneyMoney
const _melody_t melody05 = {
  120, {
    8,	E|O3,		8,	E|O3,		8,	E|O3,		8,	E|O3,		8,	E|O3,		8,	E|O3,		4,	E|O2,		4,	A|O2,
    4,	C|O3,		4,	E|O3,		8,	D_|O3,	8,	D_|O3,	8,	D_|O3,	8,	D_|O3,	8,	D_|O3,	8,	D_|O3,
    4,	F|O2,		4,	A|O2,		4,	C|O3,		4,	D_|O3,	16,	D|O3,		8,	C|O3,		8,	A|O2,		8,	C|O3,
    16,	C|O3,		32,	A|O2,		2,	A|O2,		2,	C|O3,		2,	E|O3,		8,	A|O3
  }
};

// Star Wars
const _melody_t melody06 = {
  110, {
    8,	C_|O1,	8,	C_|O1,	4,	C_|O1,	32,	F_|O1,	32,	C_|O2,	8,	B|O1,		4,	A_|O1,	8,	G_|O1,
    32,	F_|O2,	16,	C_|O2,	8,	B|O1,		4,	A_|O1,	8,	G_|O1,	32,	F_|O2,	16,	C_|O2,	8,	B|O1,
    4,	A_|O1,  8,	B|O1,		32,	G_|O1,  8,	C_|O1,	8,	C_|O1,	4,	C_|O1,	32,	F_|O1,	32,	C_|O2,
    8,	B|O1,		4,	A_|O1,	8,	G_|O1,	32,	F_|O2,	16,	C_|O2,	8,	B|O1,		4,	A_|O1,	8,	G_|O1,
    32,	F_|O2,	16,	C_|O2,	8,	B|O1,		4,	A_|O1,	8,	B|O1,		32,	G_|O1,	16,	C_|O1,	4,	C_|O1,
    32,	D_|O1,	8,	C_|O2,	8,	B|O1,		8,	A_|O1,	8,	G_|O1,	8,	F_|O1,	4,	F_|O1,	8,	G_|O1,
    4,	A_|O1,	16,	G_|O1
  }
};

// Mission Imposible
const _melody_t melody07 = {
  120, {
    2,	E|O2,		2,	F|O2,		2,	E|O2,		2,	F|O2,		2,	E|O2,		2,	F|O2,		2,	E|O2,		2,	F|O2,
    2,	E|O2,		2,	F|O2,		2,	E|O2,		2,	F|O2,		2,	E|O2,		2,	F|O2,		2,	E|O2,		2,	F|O2,
    4,	G|O2,		8,	P|O2,		4,	G|O2,		8,	P|O2,		4,	F|O2,		4,	P|O2,		4,	F_|O2,	4,	P|O2,
    4,	G|O2,		8,	P|O2,		4,	G|O2,		8,	P|O2,		4,	A_|O2,	4,	P|O2,		4,	C|O3,		4,	P|O2,
    4,	A_|O2,	4,	G|O2,		32,	D|O2,		2,	P|O2,		4,	A_|O2,	4,	G|O2,		32,	C_|O2,	2,	P|O2,
    4,	A_|O2,	4,	G|O2,		32,	C|O2,		4,	P|O2,		4,	A_|O1,	4,	C|O2
  }
};

// Frog (Croak)
const _melody_t melody08 = {
	120, {
		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		
		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		32,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,		
		2,	P|O2,		2,	A_|O3,	2,	P|O2,	  2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,		
		2,	P|O2,		32,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,	  2,	A_|O3,	2,	P|O2,		
		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		64,	P|O2,		64,	P|O2,		
		16,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,		
		2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		32,	P|O2,		2,	A_|O3,	2,	P|O2,		
		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		
		2,	A_|O3,	2,	P|O2,		32,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,		
		2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		2,	A_|O3,	2,	P|O2,		64,	P|O2,		
		64,	P|O2,		
	}
};

// Mozart 40
const _melody_t melody09 = {
	240, {
		8,	A|O2,		4,	G_|O2,	4,	P|O2,		8,	G_|O2,	8,	P|O2,		8,	A|O2,		4,	G_|O2,	4,	P|O2,		
		8,	G_|O2,	8,	P|O2,		8,	A|O2,		4,	G_|O2,	4,	P|O2,		16,	G_|O2,	12,	E|O3,		16,	P|O2,		
		4,	P|O2,		8,	E|O3,		4,	D_|O3,	4,	P|O2,		8,	C_|O3,	8,	P|O2,		8,	C_|O3,	4,	B|O2,		
		4,	P|O2,		8,	A|O2,		8,	P|O2,		6,	A|O2,		2,	P|O2,		4,	G_|O2,	4,	P|O2,		8,	F_|O2,		
		8,	P|O2,		8,	F_|O2,	16,	P|O2,		8,	P|O2,		6,	G_|O2,	2,	P|O2,		6,	F_|O2,	2,	P|O2,		
		8,	F_|O2,	8,	P|O2,		8,	G_|O2,	4,	F_|O2,	4,	P|O2,		8,	F_|O2,	8,	P|O2,		8,	G_|O2,		
		4,	F_|O2,	4,	P|O2,		16,	F_|O2,	8,	D_|O3,	16,	P|O2,		8,	P|O2,		8,	D_|O3,	8,	C_|O3,		
		8,	C|O3,		8,	P|O2,		8,	C|O3,		8,	A|O2,		6,	G_|O2,	8,	P|O2,		2,	P|O2,		8,	G_|O2,		
		8,	F_|O2,	8,	E|O2,		8,	P|O2,		8,	E|O2,		16,	P|O2,		8,	P|O2,		8,	E|O3,		4,	D_|O3,		
		4,	P|O2,		16,	D_|O3,	16,	F_|O3,	16,	C|O3,		16,	D_|O3,	16,	C_|O3,	16,	G_|O2,	16,	P|O2,		
		6,	E|O3,		2,	P|O2,		4,	D_|O3,	4,	P|O2,		16,	D_|O3,	16,	F_|O3,	16,	C|O3,		16,	D_|O3,		
		16,	C_|O3,	16,	E|O3,		8,	D_|O3,	8,	C_|O3,	8,	B|O2,		8,	A|O2,		64,	G_|O2,	64,	G|O2,		
		32,	G_|O2,	16,	P|O2,		4,	G_|O1,	4,	P|O2,		4,	G_|O1,	4,	P|O2,		32,	G_|O1,	16,	P|O2,		
		4,	G_|O1,	4,	P|O2,		4,	G_|O1,	4,	P|O2,		32,	G_|O1,	16,	P|O2,		4,	G_|O1,	4,	P|O2,		
		4,	G_|O1,	4,	P|O2,		8,	G_|O1,	8,	P|O2,		4,	G_|O1,	4,	P|O2,		4,	G_|O1,	4,	P|O2,		
		8,	G_|O1,	8,	P|O2,		4,	G_|O1,	4,	P|O2,		4,	G_|O1,	4,	P|O2,		32,	G_|O1,		
	}
};

// Toreador
const _melody_t melody10 = {
	180, {
		16,	C|O3,		12,	D|O3,		4,	C|O3,		16,	A|O2,		16,	A|O2,		8,	A|O2,		4,	P|O2,		4,	G|O2,		
		12,	A|O2,		4,	A_|O2,	16,	A|O2,		16,	P|O2,		16,	A_|O2,	12,	G|O2,		4,	C|O3,		16,	A|O2,		
		16,	P|O2,		16,	F|O2,		12,	D|O2,		4,	G|O2,		16,	C|O2,		16,	P|O2,		32,	G|O2,		8,	G|O2,		
		8,	D|O3,		8,	C|O3,		8,	A_|O2,	6,	A|O2,		2,	P|O2,		8,	G|O2,		8,	A|O2,		8,	A_|O2,		
		16,	A|O2,		16,	P|O2,		16,	E|O2,		16,	A|O2,		16,	A|O2,		8,	G_|O2,	8,	B|O2,		64,	E|O3,		
		8,	P|O2,		2,	D|O3,		4,	E|O3,		2,	D|O3,		8,	C_|O3,	8,	D|O3,		8,	G|O2,		8,	A|O2,		
		16,	A_|O2,	8,	P|O2,		2,	A|O2,		4,	A_|O2,	2,	A|O2,		8,	F|O2,		8,	D|O3,		24,	C|O3,		
		16,	P|O2,		2,	F|O2,		4,	G|O2,		2,	F|O2,		8,	C|O2,		8,	A_|O2,	8,	A|O2,		8,	P|O2,		
		8,	G|O2,		8,	P|O2,		8,	F|O2,		32,	P|O2,		
	}
};


///////////////////////////////////////////////////////////////////////////////
const _melody_t *melodies[] = {
  &melody00, &melody01, &melody02, &melody03, &melody04, &melody05, &melody06, &melody07, &melody08, &melody09,
  &melody10
};
