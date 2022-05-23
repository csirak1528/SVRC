package compressionLibs

import (
	"os"
	"time"

	"github.com/pierrec/lz4"
)

func CompressLZ4(data []byte) map[string]float64 {
	start := time.Now()
	buf := make([]byte, len(data))
	ht := make([]int, 64<<10)
	n, err := lz4.CompressBlock(data, buf, ht)
	Check(err)
	outBody := map[string]float64{"speed": 0, "ratio": 1}
	if !(n >= len(data) || n == 0) {
		o := float64(len(data))
		end := float64(time.Since(start).Seconds())
		totalTime := (o / end)
		outBody = map[string]float64{"speed": totalTime, "ratio": (float64(len(data)) / float64(n)), "size": float64(len(data) / (1024 * 1024))}

	}
	return outBody
	// Allocated a very large buffer for decompression.

}

func GetBenchmarksLZ4() map[string]interface{} {
	benchmarks := make(map[string]interface{})
	for i := range BenchmarkFileData {
		benchmarkFileType := BenchmarkFileData[i]
		testBytes, err := os.ReadFile(benchmarkFileType["path"])
		Check(err)
		filebenchmarks := CompressLZ4(testBytes)
		benchmarks[benchmarkFileType["type"]] = filebenchmarks
	}
	return map[string]interface{}{"benchmarks": benchmarks}

}
