
echo "encoding..."
./encode.exe $1.pcm $1.bc

echo "decoding..."
./decode.exe $1.bc $1

echo "generate player sample..."
ffmpeg -y -ar 8000 -ac 1 -f s8 -i $1 $1.wav

echo "finish"
