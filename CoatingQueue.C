#include "CoatingQueue.H"

CoatingElement::CoatingElement(int numi, double z)
{
  splatNumber=numi;
  splatZ=z;
}

int CoatingElement::num()
{
  return splatNumber;
  
}

double CoatingElement::sZ()
{
  return splatZ;
}



std::ostream& operator<<(std::ostream& s, const CoatingElement& el)
{
  return s << " " << el.splatNumber << " " << el.splatZ ;
}


void CoatingQueue::push_back(int num, double z)
{
  CoatingElement el(num,z);
  std::deque<CoatingElement>::push_back(el);
  return;
}



CoatingQueue& CoatingQueue::operator+(const CoatingQueue & in)
{
    error("Don´t add CoatingQueue objects");
    return (*this);
}

CoatingQueue& CoatingQueue::operator-(const CoatingQueue & in)
{
    error("Don´t subtract CoatingQueue objects");
    return (*this);
}

std::ostream& operator<<(std::ostream& s, const CoatingQueue& queue)
{
  for (int i=0; i < queue.size(); i++ )
    {
      s << " " << queue[i] << " ";
    }
  return s;
}

std::istream& operator>>(std::istream& s, const CoatingQueue& in)
{
  error("Reading into queue not yet defined!");
  return s;
}

