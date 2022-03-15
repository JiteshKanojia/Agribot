echo "Press Ctrl-C to exit"
echo "Make sure "Curl is installed and execution policy is set to Unrestricted"
echo "Created by Jitesh Kanojia"

while(1){
curl.exe http://agribot/forward
echo ""
curl.exe http://agribot/reverse
echo ""
curl.exe http://agribot/left
echo ""
curl.exe http://agribot/right
echo ""
}