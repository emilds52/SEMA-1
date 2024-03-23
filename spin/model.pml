ltl p1 { [](!(Vp && Vs)) } // Globally never Vp and Vs at the same time
ltl p2 { []<> (nxt && E) -> []<>Vp } // Globally eventually E means globally eventually Vp (?)
ltl p3 { []<> nxt -> ([](E -> <> Vs)) } // Globally if E then eventually Vp
ltl p4 { [](Vs && !E -> <>(!Vs W E)) } // Si V de la segunda y no E entonces eventualmente no se pone Vs a no ser que haya E?

int E;
int nxt;
int Rp, Ap, Vp;
int Rs, As, Vs;
int state;

mtype = { s_Vp, s_Ap, s_Rp, s_Vs, s_As, s_Rs };

active proctype fsm() {
	state = s_Vp;
	Vp = 1; Ap = 0; Rp = 0;
	Vs = 0; As = 0; Rs = 1;
	nxt = 0;
	do
	:: (state == s_Vp) -> atomic {
		if
		:: (nxt && E) -> Ap = 1; Vp = 0; nxt = 0; E = 0; state = s_Ap;
		fi
	}
	:: (state == s_Ap) -> atomic {
		if
		:: nxt -> Ap = 0; Rp = 1; nxt = 0; state = s_Rp;
		fi
	}
	:: (state == s_Rp) -> atomic {
		if
		:: nxt -> Ap = 0; Rp = 1; nxt = 0; state = s_Rp;
		fi
	}
	:: (state == s_Vs) -> atomic {
		if
		:: nxt -> As = 1; Vs = 0; nxt = 0; state = s_As;
		fi
	}
	:: (state == s_As) -> atomic {
		if
		:: nxt -> As = 0; Rs = 1; nxt = 0; state = s_Rs;
		fi
	}
	:: (state == s_Rs) -> atomic {
		if
		:: nxt -> Vp = 1; Rp = 0; nxt = 0; state = s_Vp;
		fi
	}
	od
}

active proctype entorno () {
	do
	:: if
		:: E = 1
		:: nxt = 1
		:: skip
		fi;
		printf("state = %e, E = %d, next = %d\n", state, E, nxt)
	od
}
