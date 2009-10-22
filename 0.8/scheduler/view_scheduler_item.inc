<?php
  $allday = text("Nee");
  if ($appInfo[0]["item_allday"] == 1)
  {
   $allday = text("Ja");
  }

  function normalDate($date)
  {
   return substr($date, 8,2)."-".substr($date,5,2)."-".substr($date,0,4);
  }

?>
<div align='center'>
<table border='0' cellpadding='0' cellspacing='0' width='98%'>
<tr bgcolor='#F0F0F0'>
 <td valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo $appInfo[0]["item_title"] ?></b></font></td>
 <td valign='top' align='right'><a href="javascript:window.close();"><font face='verdana' size='-1' color='#000000'><?php echo text("close") ?></font></a></td>
</tr>
</table>
<table border='0' cellpadding='1' cellspacing='1' width='98%'>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("Scheduled by") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo $appInfo[0]["item_owner"] ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("Description") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo $appInfo[0]["item_description"] ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("begins at") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo normalDate($appInfo[0]["item_begin_date"]).", ".$appInfo[0]["item_begin_time"] ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("ends at") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo normalDate($appInfo[0]["item_end_date"]).", ".$appInfo[0]["item_end_time"] ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("location") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo $appInfo[0]["item_location"] ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("type") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo $appInfo[0]["schedule_type"] ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("allday event") ?></font></td>
 <td width='80%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><b><?php echo $allday ?></b></font></td>
</tr>
<tr>
 <td width='20%' valign='top' align='left'><font color='#000000' face='verdana' size='-1'><?php echo text("attendees") ?></font></td>
 <td width='80%' valign='top' align='left'>
  <font color='#000000' face='verdana' size='-1'>
  <b>
  <?php
   if (count($attInfo) == 0)
   {
    echo text("None");
   }
   else
   {   
    for ($i=0;$i<=count($attInfo)-1;$i++)
    {
     echo $attInfo[$i]["user_id"]." ";
    }
   } 
  ?> 
  </b>
  </font>
 </td>
</tr>
</table>
</div>