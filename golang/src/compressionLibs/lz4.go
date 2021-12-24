package compressionLibs

import (
	"fmt"
	"os"

	"github.com/pierrec/lz4"
)

func CompressLZ4(data []byte) {
	buf := make([]byte, len(data))
	n, err := lz4.CompressBlockHC(data, buf, 10000)
	if err != nil {
		fmt.Println(err)
	}
	if n >= len(data) {
		fmt.Println("is not compressible")
	}
	fmt.Println(float64(len(data)) / float64(n))
	// Allocated a very large buffer for decompression.

}

const TESTPATH = "testfile.png"

func GetBenchmarksLZ4() map[string]interface{} {
	testBytes, err := os.ReadFile(TESTPATH)
	Check(err)
	CompressLZ4(testBytes)

	return map[string]interface{}{}
}
