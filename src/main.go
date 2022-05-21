package main

import (
	compression "github.com/csirak1528/svrc/golang/src/compressionLibs"
)

func main() {
	// x := make(chan float64)
	// speedNum := <-x
	// go network.Ping(x, 3)
	// fmt.Println("SVRC Initialized")
	// fmt.Println(speedNum)
	compression.InitCompression()
}
