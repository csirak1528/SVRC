package main

import (
	"encoding/json"

	"io/ioutil"
	"strconv"

	"github.com/csirak1528/svrc/golang/src/analysis"
)

func Check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {

	// fmt.Println("SVRC Initialized")
	// fmt.Println(speedNum)

	// // compressionLibs.InitCompression()

	// x := make(chan float64)

	// fmt.Println("SVRC Intialized")
	// go network.StartNetworkSpeedLiveUpdates(x)

	// for {
	// 	select {
	// 	case speedNum := <-x:
	// 		go analysis.LoadBenchmarks(speedNum)
	// 	}
	// }

	CreateVisualizationDataset()

}

func CreateVisualizationDataset() {
	body := map[string]interface{}{}
	for speed := 0; speed < 1024*1024*40; speed += 100000 {
		data := analysis.LoadBenchmarks(float64(speed))
		body[strconv.Itoa(speed)] = data
	}
	benchmarkBytes, err := json.Marshal(body)
	Check(err)
	_ = ioutil.WriteFile("./data.json", benchmarkBytes, 0644)
}
