package network

type NetworkPackage struct {
	Data      []byte
	Algorithm string
	ShardNum  int64
}

type GreetPackage struct {
	FileName string
	FileSize int64
}

type Endpoint struct {
	Ip   string
	Port int32
}

func Init() {

}

func Connect() {

}

func StartNetworkSpeedLiveUpdates(x chan float64) {
	for true {
		speed := Ping(4)
		x <- speed
	}

}
