package compressionLibs

type compressionAlgorithm interface {
	compress([]byte) []byte
	decompress([]byte) []byte
}
