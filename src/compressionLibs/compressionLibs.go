package compressionLibs

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
)

type CompressionAlgorithm struct {
	CompressionRate  int    `json:"rate"`
	CompressionRatio int    `json:"ratio"`
	Name             string `json:"name"`
}

func Check(e error) {
	if e != nil {
		panic(e)
	}
}

func InitCompression() {

	benchmarks := make(map[string]map[string]interface{})

	for i := range Supported {
		alg := Supported[i]
		benchmarks[alg] = GetBenchmarks(alg)

	}

	benchmarkBytes, err := json.Marshal(benchmarks)
	Check(err)

	_ = ioutil.WriteFile("./benchmarks.json", benchmarkBytes, 0644)
}

func getFileName(filetype string) map[string]string {
	return map[string]string{
		"type": filetype,
		"path": fmt.Sprintf("../testfiles/testfile.%s", filetype),
	}
}

func GenerateTestFiles() []map[string]string {
	pathBodies := make([]map[string]string, len(SupportedFileTypes))
	for i := range SupportedFileTypes {
		filetype := SupportedFileTypes[i]
		pathBodies[i] = getFileName(filetype)
	}
	return pathBodies
}

func GetBenchmarks(alg string) map[string]interface{} {
	switch alg {
	case "lz4":
		return GetBenchmarksLZ4()
	case "zstd":
		return GetBenchmarksZSTD()
	case "zlib":
		return GetBenchmarksZLIB()
	case "snappy":
		return GetBenchmarksSNAPPY()
	case "gzip":
		return GetBenchmarksGZIP()
	default:
		return map[string]interface{}{}
	}
}

var Supported = []string{
	"lz4", "zstd", "snappy", "zlib", "gzip",
}
var SupportedFileTypes = []string{
	"html", "json", "mp3", "png",
}

var BenchmarkFileData = GenerateTestFiles()
