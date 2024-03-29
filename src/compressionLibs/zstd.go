package compressionLibs

import (
	"os"
	"time"

	"github.com/DataDog/zstd"
)

func CompressZSTD(data []byte, level int) map[string]float64 {
	start := time.Now()
	out, err := zstd.CompressLevel(nil, data, level)
	n := len(out)
	Check(err)
	outBody := map[string]float64{"speed": 0, "ratio": 1}
	if !(n >= len(data) || n == 0) {
		o := float64(len(data))
		end := float64(time.Since(start).Seconds())
		totalTime := (o / end)
		outBody = map[string]float64{"speed": totalTime, "ratio": (float64(len(data)) / float64(n)), "size": float64(len(data))}

	}
	return outBody
}

func GetBenchmarksZSTD() map[string]interface{} {
	benchmarks := make(map[string]map[int]interface{})
	levels := 22
	for i := range BenchmarkFileData {
		benchmarkFileType := BenchmarkFileData[i]
		testBytes, err := os.ReadFile(benchmarkFileType["path"])
		Check(err)
		benchmarks[benchmarkFileType["type"]] = make(map[int]interface{})
		for j := 1; j <= levels; j++ {
			filebenchmarks := CompressZSTD(testBytes, j)
			benchmarks[benchmarkFileType["type"]][j] = filebenchmarks
		}
	}
	return map[string]interface{}{"benchmarks": benchmarks, "levels": levels}
}
