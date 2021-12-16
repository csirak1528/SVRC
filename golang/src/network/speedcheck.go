package network

import (
	"fmt"
	"net/http"
	"strconv"
	"time"

	"github.com/johnsto/speedtest"
)

func Ping(x chan float64, accuracy time.Duration) {
	// Fetch server list
	settings, _ := speedtest.FetchSettings()
	// Configure benchmark
	benchmark := speedtest.NewDownloadBenchmark(*http.DefaultClient, settings.Servers[0])
	// Run benchmark
	rate := speedtest.RunBenchmark(benchmark, 4, 16, time.Second*accuracy)
	// Print result (bps)
	fmt.Println(rate * 8 / (1024 * 1024))
	speed := speedtest.NiceRate(rate)
	//create storageArr
	nums := make([]byte, len(speed))
	endpoint := 0
	for i := range speed {
		_, err := strconv.Atoi(string(speed[i]))
		if err == nil || speed[i] == '.' {
			nums[i] = speed[i]
			endpoint = i
		} else {
			break
		}
	}
	speedNum, err := strconv.ParseFloat(string(nums[:endpoint]), 64)
	if err != nil {
		panic(err)
	}
	x <- speedNum
}
