
enum class Color 
  {RED, BLUE, GREEN} 

ostream& operator<< (ostream& out, const Color &c)
{
  switch(c)
  {
    case Color::RED:
      out << "RED";
      break;
    case Color::BLUE:
      out << "BLUE";
      break;
    case Color::GREEN:
      out << "GREEN";
      break;
  }
  
  return out;
}
