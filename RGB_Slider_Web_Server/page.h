#ifndef header_h
#define header_h

const char file[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang=en>
	<head>
		<title> RGB Slider</title>
		<meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width'/>
	</head>
	<body bgcolor=black>
		<style>
                content 
                {
                    max-width: auto;
                    height: auto;
                    width: auto;
                    margin: auto;
                }
        </style>
		<form action='write' method='get'>
		<p align=center>
		 <tr>
		 	<td>
		 		<font color=red>R &nbsp;
		 		</font>
		 	</td>
		 	<td>
		 		<input type='range' id='redInput' name='r' min='0' max='255' value='0' class='slider' oninput='rval.value=redInput.value' style=height:auto; width:400px>
		 		<font color=red>
		 			<output id='rval' name='rval' for='redInput'>&nbsp; &nbsp; 0
		 			</output>
		 		</font>
		 		</td>
		 	</tr>
		 </p>
		<br>     
		<p align=center>
			<tr>
				<td>
					<font color=lightgreen>G &nbsp;
					</font>
				</td>
				<td>
					<input type='range' id='greenInput' name='g' min='0' max='255' value='0' class='slider' oninput='gval.value=greenInput.value' style=height:auto; width:400px>
					<font color=lightgreen>
						<output id='gval' name='gval' for='greenInput'>&nbsp; &nbsp; 0
						</output>
					</font>
				</td>
			</tr>
		</p>
		<br>   
		<p align=center>
			<tr>
				<td>
					<font color=lightblue>B &nbsp;
					</font>
				</td>
				<td>
					<input type='range' id='blueInput' name='b' min='0' max='255' value='0' class='slider' oninput='bval.value=blueInput.value' style=height:auto; width:400px>
						<font color=lightblue>
							<output id='bval' name='bval' for='blueInput'>&nbsp; &nbsp; 0
							</output>
						</font>
					</td>
				</tr>
			</p>
		<br>   
		<p align=center>
			<tr>
				<td>					
				</td>
				<td>
					<input type='submit' value='WRITE' style=height:60px;width:200px />
				</td>
			</tr>
		</p>
		</form></table>
    <br>
        <h6 align=center><font color=white><a href=https://www.facebook.com/MicroArena>https://www.facebook.com/MicroArena</font></a></h6>
	</body>
</html>
)rawliteral";

#endif
