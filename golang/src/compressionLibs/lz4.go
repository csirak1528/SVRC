package compressionLibs

import (
	"fmt"
	"os"
	"time"

	"github.com/pierrec/lz4"
)

func CompressLZ4(data []byte) {
	start := time.Now()
	buf := make([]byte, len(data))
	ht := make([]int, 20<<2)
	n, err := lz4.CompressBlock(data, buf, ht)
	if err != nil {
		fmt.Println(err)
	}
	if n >= len(data) {
		fmt.Println("is not compressible")
	}
	o := float64(n) / float64(1024*1024)
	end := float64(time.Since(start).Seconds())
	fmt.Println(o / end)
	fmt.Println(float64(len(data)) / float64(n))
	// Allocated a very large buffer for decompression.

}

const TESTPATH = "../../testfiles/testfile.html"

func GetBenchmarksLZ4() map[string]interface{} {
	testBytes, err := os.ReadFile(TESTPATH)
	Check(err)
	CompressLZ4(testBytes)
	return map[string]interface{}{}
}
