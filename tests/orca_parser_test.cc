#include <type_traits>
#include <iomanip>
#include <yaml-cpp/yaml.h>
#include <Eigen/Eigen>
#include <iostream>
#include <fstream>

namespace YAML {

  template < typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
  struct convert< Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
  {
    static Node encode(const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& matrix)
    {
      Node node(NodeType::Sequence);

      // Save data given as a vector
      if (_Rows == 1 || _Cols == 1) {
        for (int row=0; row<matrix.rows(); row++)
          for (int col=0; col<matrix.cols(); col++)
    	    node.push_back(matrix(row,col));
        return node;
      }

      return node;
    }

    static bool decode(const Node& node, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& matrix)
    {
	  // Read data given as a vector
	  if (_Rows == 1 || _Cols == 1) {
	    (_Rows == 1 ? matrix.resize(_Rows, node.size()) : matrix.resize(node.size(), _Cols));
	    for (int id=0; id<node.size(); id++)
		  (node[0].size() == 0 ? matrix(id) = node[(int)id].as<_Scalar>() :  matrix(id) = node[(int)id][0].as<_Scalar>());
	    return true;
	  }

      return true;
    }
  };

} // namespace YAML

using namespace std;

int main(int argc, char *argv[]) {

    if(argc < 2)
    {
        cerr << "Usage " << argv[0] << " some_yaml_file.yml" << '\n';
        return 0;
    }
    ifstream fin;
    fin.open(argv[1]);

    YAML::Node config = YAML::Load(fin);
    cout << "-----------------" << '\n';
    cout << config << '\n';
    cout << "-----------------" << '\n';
    auto controller = config["controller"];

    if(controller)
    {
        auto tasks = controller["tasks"];

        if(tasks)
        {
            auto cart = tasks["CartTask_EE"];

            if(cart)
            {
                auto servo = cart["servo_controller"];

                if(servo)
                {
                    auto pid = servo["pid"];

                    if(pid)
                    {
                        Eigen::VectorXd p;
                        p = pid["p"].as<Eigen::VectorXd>();
                        cout << "PID P : " << p.transpose() << '\n';
                    }
                }
            }
        }
    }
    return 0;
}
