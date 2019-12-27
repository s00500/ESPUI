function lineGraph(parent, xAccessor, yAccessor) {
  // Constant size definitions TODO: this could well be improved and calculated...
  const width = 620;
  const height = 420;
  const gutter = 40;
  const pixelsPerTick = 30;

  /**
   * Creates an object that contatins transform functions that:
   *   transforms numeric data into coordinate space, linearly
   *   transforms coordinates into numeric data, linearly
   */
  function numericTransformer(dataMin, dataMax, pxMin, pxMax) {
    var dataDiff = dataMax - dataMin,
      pxDiff = pxMax - pxMin,
      dataRatio = pxDiff / dataDiff,
      coordRatio = dataDiff / pxDiff;

    return {
      // transforms a data point to a coordinate point
      toCoord: function(data) {
        return (data - dataMin) * dataRatio + pxMin;
      },
      // transforms a coord point to a data point
      toData: function(coord) {
        return (coord - pxMin) * coordRatio + dataMin;
      }
    };
  }

  /**
   * Renders an axis.
   *   orientation = 'x' or 'y'
   *   transform = a function for transforming px into data for labeling/creating tick marks
   */
  function axisRenderer(orientation, transform) {
    var axisGroup = document.createElementNS("http://www.w3.org/2000/svg", "g");
    var axisPath = document.createElementNS(
      "http://www.w3.org/2000/svg",
      "path"
    );

    axisGroup.setAttribute("class", orientation + "-axis");

    var xMin = gutter;
    var xMax = width - gutter;
    var yMin = height - gutter;
    var yMax = gutter;

    if (orientation === "x") {
      axisPath.setAttribute(
        "d",
        "M " + xMin + " " + yMin + " L " + xMax + " " + yMin
      );

      // generate labels
      for (var i = xMin; i <= xMax; i++) {
        if ((i - xMin) % pixelsPerTick === 0 && i !== xMin) {
          var text = document.createElementNS(
            "http://www.w3.org/2000/svg",
            "text"
          );
          // primitive formatting
          text.innerHTML = Math.floor(transform(i));
          text.setAttribute("x", i);
          text.setAttribute("y", yMin);
          // offset the text by 1 em
          text.setAttribute("dy", "1em");
          axisGroup.appendChild(text);
        }
      }
    } else {
      axisPath.setAttribute(
        "d",
        "M " + xMin + " " + yMin + " L " + xMin + " " + yMax
      );

      // generate labels
      for (var i = yMax; i <= yMin; i++) {
        if ((i - yMin) % pixelsPerTick === 0 && i !== yMin) {
          var tickGroup = document.createElementNS(
            "http://www.w3.org/2000/svg",
            "g"
          );
          var gridLine = document.createElementNS(
            "http://www.w3.org/2000/svg",
            "path"
          );
          text = document.createElementNS("http://www.w3.org/2000/svg", "text");
          // primitive formatting
          text.innerHTML = Math.floor(transform(i));
          text.setAttribute("x", xMin);
          text.setAttribute("y", i);
          // offset the text labels to align with grid line and keeping it to the left of the y-axis
          text.setAttribute("dx", "-.5em");
          text.setAttribute("dy", ".3em");

          gridLine.setAttribute(
            "d",
            "M " + xMin + " " + i + " L " + xMax + " " + i
          );

          tickGroup.appendChild(gridLine);
          tickGroup.appendChild(text);
          axisGroup.appendChild(tickGroup);
        }
      }
    }

    axisGroup.appendChild(axisPath);
    parent.appendChild(axisGroup);
  }

  /**
   * Renders a line
   */
  function lineRenderer(xAccessor, yAccessor, xTransform, yTransform) {
    var line = document.createElementNS("http://www.w3.org/2000/svg", "path");

    xAccessor.reset();
    yAccessor.reset();
    if (!xAccessor.hasNext() || !yAccessor.hasNext()) {
      return;
    }

    var pathString =
      "M " + xTransform(xAccessor.next()) + " " + yTransform(yAccessor.next());
    while (xAccessor.hasNext() && yAccessor.hasNext()) {
      pathString +=
        " L " +
        xTransform(xAccessor.next()) +
        " " +
        yTransform(yAccessor.next());
    }

    line.setAttribute("class", "series");
    line.setAttribute("d", pathString);

    parent.appendChild(line);
  }

  /**
   * Renders data point circles + text labels
   */
  function pointRenderer(xAccessor, yAccessor, xTransform, yTransform) {
    var pointGroup = document.createElementNS(
      "http://www.w3.org/2000/svg",
      "g"
    );

    pointGroup.setAttribute("class", "data-points");

    xAccessor.reset();
    yAccessor.reset();
    if (!xAccessor.hasNext() || !yAccessor.hasNext()) {
      return;
    }

    while (xAccessor.hasNext() && yAccessor.hasNext()) {
      var xDataValue = xAccessor.next();
      var x = xTransform(xDataValue);
      var yDataValue = yAccessor.next();
      var y = yTransform(yDataValue);

      var circle = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "circle"
      );
      circle.setAttribute("cx", x);
      circle.setAttribute("cy", y);
      circle.setAttribute("r", "4");

      var text = document.createElementNS("http://www.w3.org/2000/svg", "text");
      // primitive formatting
      text.innerHTML = Math.floor(xDataValue) + " / " + Math.floor(yDataValue);
      text.setAttribute("x", x);
      text.setAttribute("y", y);

      text.setAttribute("dx", "1em");
      text.setAttribute("dy", "-.7em");

      pointGroup.appendChild(circle);
      pointGroup.appendChild(text);
    }

    parent.appendChild(pointGroup);
  }

  // perform the rendering
  xTransform = numericTransformer(
    xAccessor.min(),
    xAccessor.max(),
    0 + gutter,
    width - gutter
  );
  // NOTE: for y... have to reverse coordinate space
  yTransform = numericTransformer(
    yAccessor.min(),
    yAccessor.max(),
    height - gutter,
    0 + gutter
  );

  axisRenderer("x", xTransform.toData);
  axisRenderer("y", yTransform.toData);

  lineRenderer(xAccessor, yAccessor, xTransform.toCoord, yTransform.toCoord);
  pointRenderer(xAccessor, yAccessor, xTransform.toCoord, yTransform.toCoord);
}

// Final render function
function renderGraphSvg(dataArray, renderId) {
  var figure = document.getElementById(renderId);
  while (figure.hasChildNodes()) {
    figure.removeChild(figure.lastChild);
  }
  //console.log(dataArray);
  var svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
  svg.setAttribute("viewBox", "0 0 640 440");
  svg.setAttribute("preserveAspectRatio", "xMidYMid meet");

  lineGraph(
    svg,
    // time accessor
    (function(data, min, max) {
      var i = 0;
      return {
        hasNext: function() {
          return i < data.length;
        },
        next: function() {
          return data[i++].x;
        },
        reset: function() {
          i = 0;
        },
        min: function() {
          return min;
        },
        max: function() {
          return max;
        }
      };
    })(
      dataArray,
      Math.min.apply(
        Math,
        dataArray.map(function(o) {
          return o.x;
        })
      ),
      Math.max.apply(
        Math,
        dataArray.map(function(o) {
          return o.x;
        })
      )
    ),
    // value accessor
    (function(data, min, max) {
      var i = 0;
      return {
        hasNext: function() {
          return i < data.length;
        },
        next: function() {
          return data[i++].y;
        },
        reset: function() {
          i = 0;
        },
        min: function() {
          return min;
        },
        max: function() {
          return max;
        }
      };
    })(
      dataArray,
      Math.min.apply(
        Math,
        dataArray.map(function(o) {
          return o.y;
        })
      ),
      Math.max.apply(
        Math,
        dataArray.map(function(o) {
          return o.y;
        })
      )
    )
  );

  figure.appendChild(svg);
}
