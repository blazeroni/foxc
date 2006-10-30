#pragma once

class IDisplayable
{
   public:
      IDisplayable();
      virtual ~IDisplayable();

      virtual void draw() = 0;
};
