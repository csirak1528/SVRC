package compressionLibs

import (
	"bytes"
	"compress/flate"
	"compress/zlib"
	"os"
	"time"
)

func CompressZLIB(data []byte, level int) map[string]float64 {
	start := time.Now()

	var out bytes.Buffer
	w := zlib.NewWriter(&out)
	w.Write(data)
	w.Close()
	n := out.Len()
	outBody := map[string]float64{"totalTime": 0, "ratio": 1}
	if !(n >= len(data) || n == 0) {
		o := float64(n) / float64(1024*1024)
		end := float64(time.Since(start).Seconds())
		totalTime := (o / end)
		outBody = map[string]float64{"totalTime": totalTime, "ratio": (float64(len(data)) / float64(n)), "speed": (float64(len(data)) / (totalTime / 1000))}
	}
	return outBody

}

func GetBenchmarksZLIB() map[string]interface{} {
	benchmarks := make(map[string]map[int]interface{})
	levels := flate.BestCompression
	for i := range BenchmarkFileData {
		benchmarkFileType := BenchmarkFileData[i]
		testBytes, err := os.ReadFile(benchmarkFileType["path"])
		Check(err)
		benchmarks[benchmarkFileType["type"]] = make(map[int]interface{})
		for j := flate.BestSpeed; j <= levels; j++ {
			filebenchmarks := CompressZLIB(testBytes, j)
			benchmarks[benchmarkFileType["type"]][j] = filebenchmarks
		}
	}
	return map[string]interface{}{"benchmarks": benchmarks}
}
