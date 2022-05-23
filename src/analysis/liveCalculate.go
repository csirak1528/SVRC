package analysis

import (
	"encoding/json"
	"io/ioutil"
	"os"
	"strconv"

	"github.com/csirak1528/svrc/golang/src/compressionLibs"
)

func Check(e error) {
	if e != nil {
		panic(e)
	}
}

//returns  map[string]interface{}
func LoadBenchmarks(speed float64) map[string]interface{} {
	benchmarks := loadBenchmarksFile()
	var algType string

	var fileType string
	body := map[string]interface{}{}
	for i := range compressionLibs.Supported {
		algType = compressionLibs.Supported[i]

		fullAlgorithmData, _ := benchmarks[algType].(map[string]interface{})
		algorithm, _ := fullAlgorithmData["benchmarks"].(map[string]interface{})

		totalLevels, levels := fullAlgorithmData["levels"].(float64)

		if levels {
			maxScore := 1.0
			maxFileType := ""
			levelSet := 0
			for j := range compressionLibs.SupportedFileTypes {
				fileType = compressionLibs.SupportedFileTypes[j]
				fileTypeData := algorithm[fileType]
				levelsData, _ := fileTypeData.(map[string]interface{})
				for k := 1; k <= int(totalLevels); k++ {
					algFileData, _ := levelsData[strconv.Itoa(k)].(map[string]interface{})
					efficiency := setSVRCAlgFileScore(algFileData, speed)
					if maxScore < efficiency {
						maxScore = efficiency
						maxFileType = fileType
						levelSet = k
					}
				}
			}
			body[algType] = map[string]interface{}{"maxScore": maxScore, "maxFileType": maxFileType, "levelSet": levelSet, "algType": algType}

		} else {
			maxScore := 1.0
			maxFileType := ""
			for j := range compressionLibs.SupportedFileTypes {
				fileType = compressionLibs.SupportedFileTypes[j]
				fileTypeData := algorithm[fileType]
				algFileData, _ := fileTypeData.(map[string]interface{})
				efficiency := setSVRCAlgFileScore(algFileData, speed)
				if maxScore < efficiency {
					maxScore = efficiency
					maxFileType = fileType
				}
			}
			body[algType] = map[string]interface{}{"maxScore": maxScore, "maxFileType": maxFileType, "algType": algType}
		}
	}
	return body
}

func loadBenchmarksFile() map[string]interface{} {
	jsonFile, err := os.Open("./benchmarks.json")
	Check(err)
	defer jsonFile.Close()
	byteValue, _ := ioutil.ReadAll(jsonFile)
	var result map[string]interface{}
	json.Unmarshal([]byte(byteValue), &result)
	return result
}

func setSVRCAlgFileScore(algFileBenchmarkData map[string]interface{}, speed float64) float64 {
	cr, _ := algFileBenchmarkData["ratio"].(float64)
	cv, _ := algFileBenchmarkData["speed"].(float64)
	tv := (speed)
	efficiency := Sctf(tv, cr, cv)

	// fmt.Println(cr, cv, tv)
	// fmt.Println(efficiency)
	return efficiency
}
