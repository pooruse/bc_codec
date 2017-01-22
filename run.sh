
echo "encoding..."
./encode.exe $1 $2

echo "decoding..."
./decode.exe $2 $3

echo "generate player sample..."
ffmpeg -y -ar 8000 -ac 1 -f s8 -i $3 $4

echo "finish"
