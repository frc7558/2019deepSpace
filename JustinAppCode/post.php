<?php 
$db_HOST = "localhost";
$db_USER = "team7558_s";
$db_PASS = "Mr.Roboto11235";
$db_NAME = "team7558_scouting";
error_reporting(E_ERROR | E_PARSE);
session_start();

$conn = new mysqli($db_HOST, $db_USER, $db_PASS, $db_NAME);

$Date = date("Y/m/d");
$Time = date("h:i:sa");
$Username = strip_tags($_POST['Username']);
$HABSandstormScore = strip_tags($_POST['HABSandstormScore']);
$SandstormCargo = strip_tags($_POST['SandstormCargo']);
$SandstormPanels = strip_tags($_POST['SandstormPanels']);
$CargoFromFloor = strip_tags($_POST['CargoFromFloor']);
$CargoFromHuman = strip_tags($_POST['CargoFromHuman']);
$PanelFromFloor = strip_tags($_POST['PanelFromFloor']);
$PanelFromHuman = strip_tags($_POST['PanelFromHuman']);

$DefenseLevel = strip_tags($_POST['DefenseLevel']);
$ShipCargo = strip_tags($_POST['ShipCargo']);
$ShipPanels = strip_tags($_POST['ShipPanels']);
$LowRocketCargo = strip_tags($_POST['LowRocketCargo']);
$LowRocketPanels = strip_tags($_POST['LowRocketPanels']);
$MidRocketCargo = strip_tags($_POST['MidRocketCargo']);
$MidRocketPanels = strip_tags($_POST['MidRocketPanels']);

$HighRocketCargo = strip_tags($_POST['HighRocketCargo']);
$HighRocketPanels = strip_tags($_POST['HighRocketPanels']);
$RobotsCarried = strip_tags($_POST['RobotsCarried']);
$HABEndScore = strip_tags($_POST['HABEndScore']);
$Comments = strip_tags($_POST['Comments']);
$TeamNumber = strip_tags($_POST['TeamNumber']);

$Competition = strip_tags($_POST['Competition']);
$_SESSION['cid'] = $Competition;
$MatchNumber = strip_tags($_POST['MatchNumber']);
$RobotStation = strip_tags($_POST['RobotStation']);
$ScoutName = strip_tags($_POST['ScoutName']);


$sql = "INSERT INTO matches (Date, Time, Username, HABSandstormScore, SandstormCargo, SandstormPanels, CargoFromFloor, CargoFromHuman, PanelFromFloor, PanelFromHuman, DefenseLevel, ShipCargo, ShipPanels, LowRocketCargo, LowRocketPanels, MidRocketCargo, MidRocketPanels, HighRocketCargo, HighRocketPanels, RobotsCarried, HABEndScore, Comments, TeamNumber, Competition, MatchNumber, RobotStation, ScoutName) VALUES ('$Date', '$Time', '$Username', '$HABSandstormScore', '$SandstormCargo', '$SandstormPanels', '$CargoFromFloor', '$CargoFromHuman', '$PanelFromFloor', '$PanelFromHuman', '$DefenseLevel', '$ShipCargo', '$ShipPanels', '$LowRocketCargo', '$LowRocketPanels', '$MidRocketCargo', '$MidRocketPanels', '$HighRocketCargo', '$HighRocketPanels', '$RobotsCarried', '$HABEndScore', '$Comments', '$TeamNumber', '$Competition', '$MatchNumber', '$RobotStation', '$ScoutName')";

$query = mysqli_query($connect, $sql);

if ($conn->query($sql) === TRUE) {
    echo "Success!";
    ?>
   <script type="text/javascript">
	window.location.href = 'https://www.scouting.team7558.com/team.php?TeamNumber=<?php echo $TeamNumber; ?>';
	</script>
    <?php
}else{
	echo "Error: " . $sql . "<br>" . $conn->error;
}

?>
