package compressionLibs

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
)

type CompressionAlgorithm struct {
	CompressionRate  int    `json:"rate"`
	CompressionRatio int    `json:"ratio"`
	Name             string `json:"name"`
}

var supported = []string{
	"lz4",
}

func Check(e error) {
	if e != nil {
		panic(e)
	}
}

func InitCompression() {

	jsonFile, err := os.Open("benchmarks.json")
	if err != nil {
		fmt.Println(err)
	}

	byteValue, _ := ioutil.ReadAll(jsonFile)
	benchmarks := make(map[string]map[string]interface{})
	json.Unmarshal([]byte(byteValue), &benchmarks)

	for i := range supported {
		alg := supported[i]
		if benchmarks[alg] == nil || len(benchmarks[alg]) == 0 {

			benchmarks[alg] = GetBenchmarks(alg)
		}
	}

	jsonFile.Close()

	benchmarkBytes, _ := json.Marshal(benchmarks)
	_ = ioutil.WriteFile("../benchmarks.json", benchmarkBytes, 0644)
}

func GetBenchmarks(alg string) map[string]interface{} {
	switch alg {
	case "lz4":
		fmt.Println("hey")
		return GetBenchmarksLZ4()
	default:
		return map[string]interface{}{}
	}

}
