classdef SensorsMeasurements < SwigRef
  methods
    function this = swig_this(self)
      this = iDynTreeMEX(3, self);
    end
    function self = SensorsMeasurements(varargin)
      if nargin==1 && strcmp(class(varargin{1}),'SwigRef')
        if ~isnull(varargin{1})
          self.swigPtr = varargin{1}.swigPtr;
        end
      else
        tmp = iDynTreeMEX(1306, varargin{:});
        self.swigPtr = tmp.swigPtr;
        tmp.SwigClear();
      end
    end
    function delete(self)
      if self.swigPtr
        iDynTreeMEX(1307, self);
        self.SwigClear();
      end
    end
    function varargout = setNrOfSensors(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1308, self, varargin{:});
    end
    function varargout = getNrOfSensors(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1309, self, varargin{:});
    end
    function varargout = resize(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1310, self, varargin{:});
    end
    function varargout = toVector(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1311, self, varargin{:});
    end
    function varargout = setMeasurement(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1312, self, varargin{:});
    end
    function varargout = getMeasurement(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1313, self, varargin{:});
    end
    function varargout = getSizeOfAllSensorsMeasurements(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(1314, self, varargin{:});
    end
  end
  methods(Static)
  end
end
