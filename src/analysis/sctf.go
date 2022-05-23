package analysis

// Sirak Compression Transfer Formula

func Sctf(tv, cr, cv float64) float64 {
	efficiency := 1 / ((1 / cr) + (tv / cv))
	return efficiency
}
