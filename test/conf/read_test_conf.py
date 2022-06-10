# sys.argv[1]    rtos    [default]
# sys.argv[2]    AT1K    [AT103]
# sys.argv[3]    dev     [evb]
import os, sys, json
import operator
from functools import reduce

BASE_PATH = os.path.abspath(os.path.dirname(__file__))


class ReadTestConfFile(object):
    def __init__(self):
        json_name = sys.argv[1]
        self.conf_file = os.path.join(BASE_PATH, "test_cases_conf/test_conf_{}.json".format(json_name))
        self.test_cases = []

    def _load_json_file(self):
        with open(self.conf_file, 'r') as conf_json:
            test_conf = json.load(conf_json)

        return test_conf

    def _get_cases_types(self, chip, board):
        cases_types = []
        test_conf = self._load_json_file()
        for key, value in test_conf.items():
            if key == chip:
                for k, v in value.items():
                    if k == board:
                        cases_types.append(v)
        return cases_types

    def get_test_cases(self):
        cases_types_list = []
        if sys.argv[2] == "AT103":
            if sys.argv[3] == "evb":
                cases_types_list = self._get_cases_types("AT103", "evb")

        for cases_type in cases_types_list:
            cases_type_list = []
            for key, value in cases_type.items():
                if len(value):
                    cases_type_list.append(value)
                    
            if len(cases_type_list):
                cases_type_list = reduce(operator.add, cases_type_list)
            self.test_cases.append(cases_type_list)

        self.test_cases = reduce(operator.add, self.test_cases)
        print(self.test_cases)


def main():
    rtcf = ReadTestConfFile()
    rtcf.get_test_cases()


if __name__ == '__main__':
    main()


