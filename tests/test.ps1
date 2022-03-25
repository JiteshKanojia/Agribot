echo "-----------------------------------------------"
echo "Press Ctrl-C to exit "
echo "Make sure Curl is installed and execution policy is set to Unrestricted ";
echo "Created by Jitesh Kanojia";
echo "-----------------------------------------------"
while(1){
echo "`n FORWARD"
curl.exe http://agribot/forward
echo "`n REVERSE"
curl.exe http://agribot/reverse
echo "`n LEFT"
curl.exe http://agribot/left
echo "`n RIGHT"
curl.exe http://agribot/right
echo "`n SPRAY PUMP"
curl.exe http://agribot/sprayboth
}