$API_URL = "https://bedroomlights.toikos.com.br/lights";
$ICON_PATH = "./icon.ico";

$oldState = "";

while($true) {
    $response = (Invoke-WebRequest -Uri $API_URL -Method GET -UseBasicParsing).Content;
    $state = ConvertTo-Json(($response | ConvertFrom-Json | select state));

    if($state -ne $oldState) {
        Add-Type -AssemblyName System.Windows.Forms;
        $global:balloon = New-Object System.Windows.Forms.NotifyIcon;
        $path = (Get-Process -id $pid).Path;
        $balloon.Icon = $ICON_PATH;
        $balloon.BalloonTipText = 'As luzes do seu quarto foram alternadas com sucesso 😉';
        $balloon.BalloonTipTitle = "Luzes alternadas";
        $balloon.Visible = $true;
        $balloon.ShowBalloonTip(5000);

        $oldState = $state;
    }
    Start-Sleep -Seconds 5;
}