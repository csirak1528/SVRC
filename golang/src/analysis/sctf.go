package analysis

//Sirak Compression Transfer Formula

func Sctf(fs, tv, cr, cv int) int {
	efficiency := (fs / tv) / (((fs / cr) / tv) + (fs / cv))
	return efficiency
}
