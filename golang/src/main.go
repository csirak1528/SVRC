package main

import (
	//"github.com/csirak1528/shvrc/golang/src/network"
	compression "github.com/csirak1528/shvrc/golang/src/compressionLibs"
)

func main() {
	// x := make(chan float64)
	// go network.Ping(x, 3)
	// fmt.Println("SHVRC Initialized")
	// speedNum := <-x
	// fmt.Println(speedNum)
	compression.InitCompression()
}
