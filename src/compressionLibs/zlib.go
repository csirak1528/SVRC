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
	w, err := zlib.NewWriterLevel(&out, level)
	Check(err)
	w.Write(data)
	w.Close()
	n := out.Len()
	outBody := map[string]float64{"speed": 0, "ratio": 1}
	if !(n >= len(data) || n == 0) {
		o := float64(len(data))
		end := float64(time.Since(start).Seconds())
		totalTime := (o / end)
		outBody = map[string]float64{"speed": totalTime, "ratio": (float64(len(data)) / float64(n)), "size": float64(len(data) / (1024 * 1024))}

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
	return map[string]interface{}{"benchmarks": benchmarks, "levels": levels}
}
