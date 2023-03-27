// temp gauge
function createVerGauge(id, minVal, maxVal, unit) {
    var tmpl = 
    '<svg class="vGauge" viewBox="0 0 145 145">'+
      '<rect class="vGauge-base" id="'+id+'_base" x="30" y="25" width="30" height="100"></rect>'+
      '<rect class="vGauge-progress" id="'+id+'_progress" x="30" y="25" width="30" height="0" fill="#1565c0"></rect>'+
      '<text class="vGauge-val" id="'+id+'_val" x="70" y="80" text-anchor="start"></text>'+
      '<text class="vGauge-min-val" id="'+id+'_minVal" x="70" y="125"></text>'+
      '<text class="vGauge-max-val" id="'+id+'_maxVal" x="70" y="30" text-anchor="start"></text>'+
    '</svg>';
    
    document.getElementById(id).innerHTML = tmpl;
    document.getElementById(id+'_minVal').textContent = minVal;
    document.getElementById(id+'_maxVal').textContent = maxVal;
    
    var gauge = {
      setVal: function(val) {
        val = Math.max(minVal, Math.min(val, maxVal));
        var height = 100/(maxVal-minVal) * (val-minVal);
        document.getElementById(id+'_progress').setAttribute("height", height);
        document.getElementById(id+'_progress').setAttribute("y", 25+(100-height));
        document.getElementById(id+'_val').textContent = val + (unit !== undefined ? unit: '');
        return gauge;
      },
      setColor: function(color) {
         document.getElementById(id+'_progress').setAttribute("fill", color);
         return gauge;
      }
    }
    
    return gauge;
  }

// temp gauge
function createVerGauge2(id, minVal, maxVal, unit) {
  var tmpl = 
  '<svg class="vGauge" viewBox="0 0 145 145">'+
    '<rect class="vGauge-base" id="'+id+'_base" x="30" y="25" width="30" height="100"></rect>'+
    '<rect class="vGauge-progress" id="'+id+'_progress" x="30" y="25" width="30" height="0" fill="#1565c0"></rect>'+
    '<text class="vGauge-val" id="'+id+'_val" x="70" y="80" text-anchor="start"></text>'+
    '<text class="vGauge-min-val" id="'+id+'_minVal" x="70" y="125"></text>'+
    '<text class="vGauge-max-val" id="'+id+'_maxVal" x="70" y="30" text-anchor="start"></text>'+
  '</svg>';
  
  document.getElementById(id).innerHTML = tmpl;
  document.getElementById(id+'_minVal').textContent = minVal;
  document.getElementById(id+'_maxVal').textContent = maxVal;
  
  var gauge = {
    setVal: function(val) {
      val = Math.max(minVal, Math.min(val, maxVal));
      var height = 100/(maxVal-minVal) * (val-minVal);
      document.getElementById(id+'_progress').setAttribute("height", height);
      document.getElementById(id+'_progress').setAttribute("y", 25+(100-height));
      document.getElementById(id+'_val').textContent = val + (unit !== undefined ? unit: '');
      return gauge;
    },
    setColor: function(color) {
       document.getElementById(id+'_progress').setAttribute("fill", color);
       return gauge;
    }
  }
  
  return gauge;
}